# Passive Buzzer Melody

A passive buzzer plays a short melody from the video game *The Legend of Zelda: Ocarina of Time*.
The notes are my own transcription from sheet music, made to practice reading notes and using arrays.

## Components
- ESP32
- Passive buzzer (open bottom, both legs the same length)
- Breadboard and jumper wires

## Wiring

| Part | Connection |
|---|---|
| Buzzer, one leg | D33 |
| Buzzer, other leg | GND |

## Files
- `10-passive-buzzer-melody.ino`: the program
- `pitches.h`: note names with their frequencies in Hz (`NOTE_D4`, `NOTE_A4`, ...) and `REST` for a pause. It must be in the same folder as the sketch.

## How it works
- Two arrays of the same length describe the melody: `melody[]` holds the notes and `durations[]` holds how long each one lasts.
- The sheet music is in 6/8, so lengths are counted in **eighth notes** (1 = eighth, 2 = quarter, 3 = dotted quarter, 4 = half, 6 = dotted half). Every bar adds up to 6, which is an easy way to check the transcription.
- `eighth` is the length of one eighth note in milliseconds, so the tempo changes with one number (smaller = faster).
- `tone(pin, frequency, duration)` plays a note, `noTone()` stops it, and a short delay after each note separates the notes.
- `sizeof(melody) / sizeof(melody[0])` counts the notes, so the number of notes is not typed by hand.
- The melody repeats after a 2 second pause.

## Limits
- The program uses `delay()`, so it cannot do anything else while the melody plays. A `millis()` version would fix that.
- One passive buzzer plays only one note at a time.

## What I learned
- Reading sheet music: clef, octaves, note lengths and time signature
- Parallel arrays and `sizeof` to count elements
- Splitting code into a header file (`pitches.h`) and including it
- Debugging typos: a missing comma in an array, a missing note definition

## Source
Melody: *Song of Storms* from The Legend of Zelda: Ocarina of Time (© Nintendo). This repository contains only my transcription for learning, not the original recording.
Code: own.
`pitches.h`: note frequency table from the HiBit Arduino pitches library (https://www.hibit.dev), copyright notice kept in the file.