"""Create an original, quiet, looping fairytale ambience with no downloaded music."""
import argparse
import math
import struct
import wave
from pathlib import Path

OUTPUT = Path(__file__).resolve().parents[1] / "assets/audio/forest_ambience.wav"
RATE, DURATION = 22050, 24


def generate(path=OUTPUT):
    path.parent.mkdir(parents=True, exist_ok=True)
    notes = (523.25, 659.25, 783.99, 987.77, 880.0, 783.99, 659.25, 587.33,
             523.25, 783.99, 659.25, 880.0, 783.99, 659.25, 587.33, 523.25)
    samples = bytearray()
    for i in range(RATE * DURATION):
        t = i / RATE
        fade = min(1.0, t / .65, (DURATION - t) / .65)
        pad = .027 * (math.sin(math.tau * 130.8128 * t)
                      + .65 * math.sin(math.tau * 196.0 * t)
                      + .40 * math.sin(math.tau * 261.6256 * t))
        chime = 0.0
        event = int(t / 1.5)
        for index in range(max(0, event - 2), event + 1):
            age = t - index * 1.5
            envelope = min(1.0, age / .025) * math.exp(-age * 1.7)
            frequency = notes[index % len(notes)]
            chime += .075 * envelope * (math.sin(math.tau * frequency * age)
                                        + .17 * math.sin(math.tau * frequency * 2.001 * age))
        value = max(-.28, min(.28, fade * (pad + chime)))
        samples.extend(struct.pack("<h", int(value * 32767)))
    with wave.open(str(path), "wb") as sound:
        sound.setnchannels(1)
        sound.setsampwidth(2)
        sound.setframerate(RATE)
        sound.writeframes(samples)


def verify(path=OUTPUT):
    with wave.open(str(path), "rb") as sound:
        assert sound.getnchannels() == 1
        assert sound.getsampwidth() == 2
        assert sound.getframerate() == RATE
        assert sound.getnframes() == RATE * DURATION
    print("PASS: original 24-second PCM ambience, 22050 Hz mono")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    if not args.check:
        generate()
    verify()
