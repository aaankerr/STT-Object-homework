import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent / "vendor"))
import argparse
import json
import queue
import time
from paho.mqtt import client as mqtt_client
from dataclasses import dataclass
from typing import Optional
import numpy as np
import sounddevice as sd
from vosk import KaldiRecognizer, Model as VoskModel
from openwakeword.model import Model as OWWModel
CHANNELS = 1
DTYPE = "int16"
CAPTURE_FRAME_MS = 80
MODEL_SAMPLE_RATE = 16000
MODEL_FRAME_SAMPLES = MODEL_SAMPLE_RATE * CAPTURE_FRAME_MS // 1000  # 1280

@dataclass
class Config:
    vosk_model: str
    wakeword_model: Optional[str]
    wakeword_name: str
    threshold: float
    silence_timeout: float
    max_record_seconds: float
    device: Optional[int]
    sample_rate: int
    debug_wake: bool

audio_queue: "queue.Queue[bytes]" = queue.Queue()
BROKER_IP = "10.21.60.231"
TOPIC = "robot/speech/text"

mqtt = mqtt_client.Client()
mqtt.connect(BROKER_IP, 1883, 60)
def audio_callback(indata, frames, time_info, status) -> None:
    if status:
        print(status)

    audio_queue.put(bytes(indata))
def build_openwakeword_model(wakeword_model_path: Optional[str]) -> OWWModel:
    if wakeword_model_path:
        return OWWModel(wakeword_models=[wakeword_model_path])
    return OWWModel()

def rms_level(audio_bytes: bytes) -> int:
    if not audio_bytes:
        return 0
    samples = np.frombuffer(audio_bytes, dtype=np.int16).astype(np.float32)
    if samples.size == 0:
        return 0
    return int(np.sqrt(np.mean(samples * samples)))

def get_wake_score(prediction: dict, wakeword_name: str) -> float:
    if wakeword_name in prediction:
        return float(prediction[wakeword_name])
    for key, value in prediction.items():
        if wakeword_name.lower() in key.lower():
            return float(value)
    return 0.0

def pick_input_device(cfg: Config) -> int:
    if cfg.device is not None:
        return cfg.device
  
    try:
        default_devices = sd.default.device
        if isinstance(default_devices, (list, tuple)) and len(default_devices) > 0:
            default_input = default_devices[0]
        else:
            default_input = default_devices
    except Exception:
        default_input = None
    if default_input in (None, -1):
        raise RuntimeError(
            "No default input device found. Run the script with --device N."
        )
    return int(default_input)

def validate_input_settings(device: int, sample_rate: int) -> None:
    sd.check_input_settings(
        device=device,
        samplerate=sample_rate,
        channels=CHANNELS,
        dtype=DTYPE,
    )

def resample_int16_mono(audio_bytes: bytes, src_rate: int, dst_rate: int) -> np.ndarray:
    samples = np.frombuffer(audio_bytes, dtype=np.int16)
    if samples.size == 0:
        return np.array([], dtype=np.int16)
    if src_rate == dst_rate:
        return samples.copy()
    src_len = len(samples)
    dst_len = int(round(src_len * dst_rate / src_rate))
    if dst_len <= 0:
        return np.array([], dtype=np.int16)
    src_x = np.linspace(0, src_len - 1, num=src_len, dtype=np.float32)
    dst_x = np.linspace(0, src_len - 1, num=dst_len, dtype=np.float32)
    resampled = np.interp(dst_x, src_x, samples.astype(np.float32))
    resampled = np.clip(resampled, -32768, 32767).astype(np.int16)
    return resampled

def run_session(cfg: Config) -> None:
    device = pick_input_device(cfg)
    validate_input_settings(device, cfg.sample_rate)
    capture_frame_samples = cfg.sample_rate * CAPTURE_FRAME_MS // 1000
    wake_model = build_openwakeword_model(cfg.wakeword_model)
    vosk_model = VoskModel(cfg.vosk_model)

    print("Listening for wake word...")
    with sd.RawInputStream(
        samplerate=cfg.sample_rate,
        blocksize=capture_frame_samples,
        device=device,
        dtype=DTYPE,
        channels=CHANNELS,
        callback=audio_callback,
    ):
        state = "waiting_for_wake"
        recognizer = None
        utterance_started_at = 0.0
        last_voice_time = 0.0
        while True:
            raw_audio = audio_queue.get()
            processed_audio = resample_int16_mono(
                raw_audio,
                src_rate=cfg.sample_rate,
                dst_rate=MODEL_SAMPLE_RATE,
            )
            if processed_audio.size == 0:
                continue
            processed_bytes = processed_audio.tobytes()
            if state == "waiting_for_wake":
                prediction = wake_model.predict(processed_audio)
                score = get_wake_score(prediction, cfg.wakeword_name)
                if cfg.debug_wake:
                    print(json.dumps({
                        "event": "wake_debug",
                        "scores": {k: round(float(v), 4) for k, v in prediction.items()}
                    }))
                if score >= cfg.threshold:
                    print(json.dumps({
                        "event": "wakeword_detected",
                        "wakeword": cfg.wakeword_name,
                        "score": round(score, 4),
                        "timestamp": time.time(),
                    }))
                    recognizer = KaldiRecognizer(vosk_model, MODEL_SAMPLE_RATE)
                    recognizer.SetWords(True)
                    state = "recording_command"
                    utterance_started_at = time.time()
                    last_voice_time = time.time()
                    recognizer.AcceptWaveform(processed_bytes)
                    continue

            elif state == "recording_command":
                recognizer.AcceptWaveform(processed_bytes)
                level = rms_level(processed_bytes)
                if level > 300:
                    last_voice_time = time.time()
                now = time.time()
                silence_elapsed = now - last_voice_time
                total_elapsed = now - utterance_started_at
                if (
                    silence_elapsed >= cfg.silence_timeout
                    or total_elapsed >= cfg.max_record_seconds
                ):
                    final_result = json.loads(recognizer.FinalResult())
                    text = final_result.get("text", "").strip()

                    if text:
                        message = {
                            "text": text,
                            "source": "stt"
                        }

                        mqtt.publish(
                            TOPIC,
                            json.dumps(message)
                        )

                        print(f"[STT → MQTT]: {message}")

                    recognizer = None
                    state = "waiting_for_wake"
                    print("Listening for wake word...")

                    
def parse_args() -> Config:
    parser = argparse.ArgumentParser()
    parser.add_argument("--vosk-model", required=True)
    parser.add_argument("--wakeword-model", default=None)
    parser.add_argument("--wakeword-name", default="alexa")
    parser.add_argument("--threshold", type=float, default=0.5)
    parser.add_argument("--silence-timeout", type=float, default=1.2)
    parser.add_argument("--max-record-seconds", type=float, default=8.0)
    parser.add_argument("--device", type=int, default=None)
    parser.add_argument("--sample-rate", type=int, default=48000)
    parser.add_argument("--debug-wake", action="store_true")
    args = parser.parse_args()
    return Config(
        vosk_model=args.vosk_model,
        wakeword_model=args.wakeword_model,
        wakeword_name=args.wakeword_name,
        threshold=args.threshold,
        silence_timeout=args.silence_timeout,
        max_record_seconds=args.max_record_seconds,
        device=args.device,
        sample_rate=args.sample_rate,
        debug_wake=args.debug_wake,
    )
if __name__ == "__main__":
    config = parse_args()
    run_session(config)

