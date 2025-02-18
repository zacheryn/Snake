# C++ Snake

## Description

This is a console based Snake game for Windows constrolled with the arrow keys. There is support for audio and high score tracking.

## High Score Tracking

The high score is tracked through `highscore.txt` in the same directory as `Snake.exe`.

### If `highscore.txt` does not exist

If the file does not exist, it will be created upon the end of the game, and the player will be told that they set the first highscore.

### If `highscore.txt` exists, but is formatted incorrectly

In the case that `highscore.txt` exists, but is incorrectly formatted, the player will be notified at the end of the game and be given the option to allow `Snake.exe` to overwrite the previous file with the new high score. From here, the behaviour is the same as if the file did not exist.

### If `highscore.txt` exists, and is formatted correctly

In the case that `highscore.txt` exists, and is correctly formatted, at the end of the game the player's score for the current game will be compared to the previous high score. If the are below, the player will be told the previous high score, and how much more score they need to break it. If the player beats the previous high score, they will be told the previous highscore, and by how much they beat it.
