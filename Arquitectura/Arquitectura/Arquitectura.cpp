#include "tigr.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Tigr* screen;
Tigr* backdrop;

float playerx = 160, playery = 120;
float playerxs = 0, playerys = 0;
int standing = 0;

// Struct to record position and actions of the player
typedef struct {
    float time;
    int leftPressed;
    int rightPressed;
    int spacePressed;
    float playerx, playery, playerxs, playerys;
} InputRecord;

#define MAX_RECORDINGS 1000
// Input Buffer for the inputs
InputRecord inputBuffer[MAX_RECORDINGS];
int bufferIndex = 0;
int recordingMode = 0; // 0 = plays normally, 1 = plays the recording

float remaining = 10.0f;

// Get all inputs of the player (Maybe make a gamepad struct here?) (to pass in the parameters?)
void recordInput(float dt, int leftPressed, int rightPressed, int spacePressed, float playerx, float playery, float playerxs, float playerys) {
    if (bufferIndex < MAX_RECORDINGS) {
        inputBuffer[bufferIndex].time = dt;
        inputBuffer[bufferIndex].leftPressed = leftPressed;
        inputBuffer[bufferIndex].rightPressed = rightPressed;
        inputBuffer[bufferIndex].spacePressed = spacePressed;
        inputBuffer[bufferIndex].playerx = playerx;
        inputBuffer[bufferIndex].playery = playery;
        inputBuffer[bufferIndex].playerxs = playerxs;
        inputBuffer[bufferIndex].playerys = playerys;
        bufferIndex++;
    }
}

// Function to play back the recorded inputs.
void playbackInput(float dt) {
    static int playbackIndex = 0;
    static float playbackTime = 0.0f;

    if (playbackIndex < bufferIndex) {
        playbackTime += dt;

        if (playbackTime >= inputBuffer[playbackIndex].time) {
            playerx = inputBuffer[playbackIndex].playerx;
            playery = inputBuffer[playbackIndex].playery;
            playerxs = inputBuffer[playbackIndex].playerxs;
            playerys = inputBuffer[playbackIndex].playerys;
            playbackIndex++;
        }
    }
}

void toggleMode() {
    if (tigrKeyDown(screen, 'R')) {
        recordingMode = !recordingMode;
    }
}

void update(float dt) {
    if (remaining > 0)
        remaining -= dt;

    toggleMode();

    if (recordingMode) {
        playbackInput(dt);
    }
    else {
        // read gamepad here basically
        // keep position using this
        if (tigrKeyHeld(screen, TK_LEFT)) {
            playerxs -= 10;
        }
        if (tigrKeyHeld(screen, TK_RIGHT)) {
            playerxs += 10;
        }
        if (tigrKeyDown(screen, TK_SPACE)) {
            playerys -= 200;
        }

        recordInput(dt, tigrKeyHeld(screen, TK_LEFT), tigrKeyHeld(screen, TK_RIGHT), (tigrKeyDown(screen, TK_SPACE)), playerx, playery, playerxs, playerys);
    }

    // Apply physics.
    playerxs *= exp(-10.0f * dt);
    playerys *= exp(-2.0f * dt);
    playerys += dt * 200.0f;
    playerx += dt * playerxs;
    playery += dt * playerys;

    // Apply collision.
    if (playerx < 8) {
        playerx = 8;
        playerxs = 0;
    }

    if (playerx > screen->w - 8) {
        playerx = screen->w - 8.0f;
        playerxs = 0;
    }

    // Apply playfield collision and stepping.
    float oldx = playerx, oldy = playery;
    float dx = (playerx - oldx) / 10;
    float dy = (playery - oldy) / 10;
    standing = false;
    for (int n = 0; n < 10; n++) {
        TPixel p = tigrGet(backdrop, (int)oldx, (int)oldy - 1);
        if (p.r == 0 && p.g == 0 && p.b == 0)
            oldy -= 1;
        p = tigrGet(backdrop, (int)oldx, (int)oldy);
        if (p.r == 0 && p.g == 0 && p.b == 0 && playerys > 0) {
            playerys = 0;
            dy = 0;
            standing = true;
        }
        oldx += dx;
        oldy += dy;
    }

    playerx = oldx;
    playery = oldy;
}

int main(int argc, char* argv[]) {
    // Load our sprite.
    Tigr* squinkle = tigrLoadImage("squinkle.png");
    if (!squinkle)
        tigrError(0, "Cannot load squinkle.png");

    // Make a window and an off-screen backdrop.
    screen = tigrWindow(320, 240, "Record Demo", TIGR_2X);
    backdrop = tigrBitmap(screen->w, screen->h);

    // Fill in the background.
    tigrClear(backdrop, tigrRGB(80, 180, 255));
    tigrFill(backdrop, 0, 200, 320, 40, tigrRGB(60, 120, 60));
    tigrFill(backdrop, 0, 200, 320, 3, tigrRGB(0, 0, 0));
    tigrLine(backdrop, 0, 201, 320, 201, tigrRGB(255, 255, 255));

    // Enable post fx
    tigrSetPostFX(screen, 1, 1, 1, 2.0f);

    // Main game loop
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        // Update the game.
        float dt = tigrTime();
        update(dt);

        // Composite the backdrop and sprite onto the screen.
        tigrBlit(screen, backdrop, 0, 0, 0, 0, backdrop->w, backdrop->h);
        tigrBlitAlpha(screen, squinkle, (int)playerx - squinkle->w / 2, (int)playery - squinkle->h, 0, 0, squinkle->w,
            squinkle->h, 1.0f);

        // Display current mode
        if (recordingMode)
            tigrPrint(screen, tfont, 10, 222, tigrRGBA(0xff, 0xff, 0xff, 0xff), "Recording Mode (R to toggle)");
        else
            tigrPrint(screen, tfont, 10, 222, tigrRGBA(0xff, 0xff, 0xff, 0xff), "Player Mode (R to toggle)");

        // Update the window.
        tigrUpdate(screen);
    }

    tigrFree(squinkle);
    tigrFree(backdrop);
    tigrFree(screen);
    return 0;
}
