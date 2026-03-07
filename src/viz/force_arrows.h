#ifndef FORCE_ARROWS_H
#define FORCE_ARROWS_H

typedef struct {
    float position[3];
    float direction[3];
    float magnitude;
    float color[4];
} ForceArrow;

typedef struct {
    ForceArrow *arrows;
    int count;
    float scale;
} ForceArrows;

ForceArrows* force_arrows_create(void);
void force_arrows_destroy(ForceArrows *fa);
void force_arrows_add(ForceArrows *fa, float *pos, float *dir, float mag);
void force_arrows_clear(ForceArrows *fa);
void force_arrows_set_scale(ForceArrows *fa, float scale);
void force_arrows_render(ForceArrows *fa);

#endif
