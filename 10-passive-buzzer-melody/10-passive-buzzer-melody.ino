// Song of Storms, own transcription for learning

#include "pitches.h"

const int buzPin = 33;      // the number of the buzzer pin
const int eighth = 200;     // length of one eighth note in ms (smaller = faster)

int melody[] = {
  NOTE_D4, NOTE_A4, NOTE_D5,
  NOTE_D4, NOTE_A4, NOTE_D5,
  NOTE_E5, NOTE_F5, NOTE_E5, NOTE_F5,
  NOTE_E5, NOTE_C5, NOTE_A4,

  NOTE_C5, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_A4,
  NOTE_A4, NOTE_D4, NOTE_F4, NOTE_G4,
  NOTE_E4,
  NOTE_D4, NOTE_A4, NOTE_D5,
};

// lengths counted in eighth notes: 1 = eighth, 2 = quarter, 3 = dotted quarter, 4 = half
// every bar in 6/8 must add up to 6
int durations[] = {
  1, 1, 4,
  1, 1, 4,
  3, 1, 1, 1,
  1, 1, 4,

  2, 2, 1, 1,
  6,
  2, 2, 1, 1,
  6,
  1, 1, 4
};

void setup() {

}

void loop() {
  int size = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < size; i++) {
    int duration = durations[i] * eighth;

    if (melody[i] == REST) {
      noTone(buzPin);
    } else {
      tone(buzPin, melody[i], duration);
    }

    // a short gap between the notes
    delay(duration * 1.1);
    noTone(buzPin);
  }

  // pause before the melody repeats
  delay(2000);
}