Reaksjonsspill — Two-Player Reaction Game 🕹️

A two-player reaction game built on Arduino in C++. After a random delay a buzzer sounds — the first player to hit their button after the beep scores a point. React too early, before the buzzer, and you lose one. Each player's score is shown live on its own 7-segment display.

Under the hood it's a small finite state machine with non-blocking timing, so the game stays responsive while it drives the displays, LEDs, buzzer and a sweeping servo all at once.

How it works:

The game loops through four states:
IDLE — all LEDs off; a random wait time (2–5 s) is chosen and the round begins.
WAIT_FOR_SIGNAL — waiting for the buzzer. Press now (a false start) and you lose a point.
REACTION — the buzzer has sounded; the first player to press wins the round.
ROUND_END — a short pause, then back to IDLE for the next round.

Scores are shown on two single-digit 7-segment displays, driven by multiplexing — the two displays share the same segment pins and are switched on and off fast enough to look continuously lit. A servo sweeps back and forth as a moving indicator while the game runs.

Hardware / components:
1 × Arduino Uno (uses nearly every I/O pin)
2 × push buttons (one per player) + 1 × reset button
4 × LEDs — a green and a red per player (correct / false-start feedback)
1 × buzzer (the "go" signal)
2 × single-digit 7-segment displays (multiplexed)
1 × servo motor (moving indicator)
Current-limiting resistors for the LEDs, wires, breadboard

Buttons use the Arduino's internal pull-ups (INPUT_PULLUP), so they're wired to ground and read as LOW when pressed — no external pull-down resistors needed.

Wiring:
Component	Arduino pin(s)
Player 1 button	D7
Player 2 button	D6
Reset button	D5
Player 1 — green / red LED	D4 / D2
Player 2 — green / red LED	D3 / D8
Buzzer	A0
Servo	D9
7-segment segments A–G	D10, D11, D12, D13, A3, A2, A1
Display 1 / Display 2 enable	A4 / A5

How to run it:
Open Reaksjonsspill.ino in the Arduino IDE (or PlatformIO).
Connect the Arduino via USB and select the correct board and port.
The sketch uses the built-in Servo library (already included with the IDE).
Click Upload, wire up the components as above, and play.

Game rules:
Correct press (first to press after the buzzer): +1 point.
False start (press before the buzzer): −1 point (a score never drops below 0).
Reaching 10 points resets the game for a fresh match.
The reset button clears both scores at any time.

Possible improvements:
Show the winner when a player reaches 10, instead of silently resetting.
Measure and display reaction time in milliseconds, not just points.
Handle a truly simultaneous press fairly (see notes in the code).
Add button debouncing for extra robustness.

About:
Built as a student project while studying Computer Engineering (dataingeniør) at USN, specializing in Cyber Physical Devices.
