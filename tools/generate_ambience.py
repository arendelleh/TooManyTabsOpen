"""Create an original soft major-key harp melody, without drones or beating tones."""
import argparse
import math
import struct
import wave
from pathlib import Path

OUTPUT = Path(__file__).resolve().parents[1] / "assets/audio/forest_ambience.wav"
RATE, DURATION = 22050, 24


def generate(path=OUTPUT):
    path.parent.mkdir(parents=True, exist_ok=True)
    # Eight short bars, with a gentle C-F-C-G / C-F-G-C cadence.
    melody = (523.25,659.25,783.99,659.25, 698.46,880.0,783.99,698.46,
              659.25,783.99,659.25,523.25, 587.33,783.99,587.33,493.88,
              523.25,659.25,783.99,1046.50, 880.0,783.99,698.46,880.0,
              783.99,587.33,493.88,587.33, 659.25,587.33,523.25,523.25)
    chords = ((261.63,329.63,392.0), (349.23,440.0,523.25),
              (261.63,329.63,392.0), (196.0,246.94,293.66),
              (261.63,329.63,392.0), (349.23,440.0,523.25),
              (196.0,246.94,293.66), (261.63,329.63,392.0))
    pattern = (0,1,2,1,0,1,2,1)

    def harp(frequency, age):
        attack = min(1.0, age / .012)
        phase = math.tau * frequency * age
        return attack * (math.sin(phase) * math.exp(-age * 3.2)
                         + .22 * math.sin(phase * 2) * math.exp(-age * 5.5)
                         + .06 * math.sin(phase * 3) * math.exp(-age * 8))

    samples = bytearray()
    for i in range(RATE * DURATION):
        t = i / RATE
        fade = min(1.0, t / .1, (DURATION - t) / .4)
        music = 0.0
        event = int(t / .75)
        for index in range(max(0, event - 2), event + 1):
            music += .075 * harp(melody[index % len(melody)], t - index * .75)
        arp = int(t / .375)
        for index in range(max(0, arp - 2), arp + 1):
            chord = chords[(index // 8) % 8]
            music += .025 * harp(chord[pattern[index % 8]], t - index * .375)
        value = max(-.25, min(.25, fade * music))
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
        frames = sound.readframes(sound.getnframes())
        peak = max(abs(value[0]) for value in struct.iter_unpack("<h", frames))
        assert 0 < peak < 32767 * .26, "Sound should remain quiet and unclipped."
    print("PASS: original 24-second harp melody, 22050 Hz mono, quiet and unclipped")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    if not args.check:
        generate()
    verify()
