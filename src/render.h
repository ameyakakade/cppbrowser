#include "layout.h"

int WINDOW_HEIGHT = 900;
int WINDOW_WIDTH  = 1600;

int ywindow = 0;

bool inView(layoutNode* node, int yOffset);
void renderLayoutTree(layoutNode* node, int yOffset);
void renderLayoutTreeDebug(layoutNode* node, int yOffset);
layoutNode* hitDetect(layoutNode* node, int x, int y);
