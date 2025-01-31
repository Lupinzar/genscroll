#include <genesis.h>
#include <resources.h>

#define HCOUNT 8
//32 or 20 (cell width)?
#define VS_COL_MAX 20

void init();
void reset(bool);
void call_init();
void call_update();
void handle_input();
void button_event(u16, u16, u16);
void write_position();
void reset_last_column();
void set_column(u16);

void line_test_init();
void line_test_update();
void line_plane_test_init();
void line_plane_test_update();
void tile_test_init();
void tile_test_update();
void screen_test_init();
void screen_test_update();
void full_test_init();
void full_test_update();

enum TEST_MODE{
    TEST_LINE,
    TEST_LINE_PLANE,
    TEST_TILE,
    TEST_SCREEN,
    TEST_FULL,
    TEST_END
};

const char* TEST_MODE_NAMES[] = {
    "LINE TEST",
    "LINE PLANE TEST",
    "TILE TEST",
    "SCREEN TEST",
    "FULL TEST"
};

enum TEST_MODE mode;
s16 offset_x = 0;
s16 offset_y = 0;
bool needs_update = FALSE;
bool half_tick = FALSE;
u16 current_column = 0;
u16 last_column = 0;

int main()
{   
    init();
    while(1){
        handle_input();
        if(needs_update){
            call_update();
            write_position();
            needs_update = FALSE;
        }
        SYS_doVBlankProcess();
        half_tick = !half_tick;
    }
    return 0;
}

void init() {
    JOY_setEventHandler(button_event);
    mode = TEST_LINE;
    reset(false);
    call_init(false);
    call_update();
}

void reset(bool clear){
    offset_x = offset_y = 0;
    current_column = last_column = 0;
    if(clear){
        VDP_resetScreen();
    }
    
    VDP_setPlaneSize(32, 32, TRUE);
    PAL_setPalette(PAL0, bg1.palette->data, DMA);
    PAL_setColor(0x11, RGB3_3_3_TO_VDPCOLOR(7, 7, 7));
    PAL_setColor(0x12, RGB3_3_3_TO_VDPCOLOR(0, 0, 0));
    u16 index = TILE_USER_INDEX;
    index += bg1.tileset->numTile;
    VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, index), 0, 0, FALSE, TRUE);
    VDP_setTextPlane(BG_A);
    VDP_setTextPalette(PAL1);
    VDP_loadFont(&font, DMA);
}

void call_init(){
    switch(mode){
        case TEST_LINE:
            line_test_init();
            break;
        case TEST_LINE_PLANE:
            line_plane_test_init();
            break;
        case TEST_TILE:
            tile_test_init();
            break;
        case TEST_SCREEN:
            screen_test_init();
            break;
        case TEST_FULL:
            full_test_init();
            break;
    }
    VDP_clearText(0, 25, 16);
    VDP_drawText(TEST_MODE_NAMES[mode], 1, 25);
}

void call_update(){
    switch(mode){
        case TEST_LINE:
            line_test_update();
            break;
        case TEST_LINE_PLANE:
            line_plane_test_update();
            break;
        case TEST_TILE:
            tile_test_update();
            break;
        case TEST_SCREEN:
            screen_test_update();
            break;
        case TEST_FULL:
            full_test_update();
            break;
    }
}

void handle_input(){
    if(half_tick){
        return;
    }
    u16 value = JOY_readJoypad(JOY_1);
    if(value & BUTTON_RIGHT){
        offset_x++;
        needs_update = TRUE;
    }
    if(value & BUTTON_LEFT){
        offset_x--;
        needs_update = TRUE;
    }
    if(value & BUTTON_UP){
        offset_y++;
        needs_update = TRUE;
    }
    if(value & BUTTON_DOWN){
        offset_y--;
        needs_update = TRUE;
    }
}

void button_event(u16 joy, u16 changed, u16 state){
    if(joy != JOY_1){
        return;
    }

    if(changed & BUTTON_A){
        if(state & BUTTON_A){
            mode = (mode + 1) % TEST_END;
            reset(true);
            call_init();
            needs_update = TRUE;
        }
    }
    if(changed & BUTTON_B){
        if(state & BUTTON_B){
            offset_x = 0;
            offset_y = 0;
            set_column(0);
            reset_last_column();
            needs_update = TRUE;
        }
    }
    if(changed & BUTTON_C){
        if(state & BUTTON_C){
            set_column(current_column + 1);
            reset_last_column();
            needs_update = TRUE;
        }
    }
}

void write_position(){
    char buffer[32];
    sprintf(buffer, "%d,%d,%d", current_column, offset_x, offset_y);
    VDP_clearText(0, 26, 20);
    VDP_drawText(buffer, 1, 26);
}

void set_column(u16 column){
    last_column = current_column;
    current_column = column % VS_COL_MAX;
}

void reset_last_column(){
    s16 vvals[] = {0};
    VDP_setVerticalScrollTile(BG_B, last_column, vvals, 1, DMA);
}

void line_test_init(){
    VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_COLUMN);
}

void line_test_update(){
    s16 vvals[] = {offset_y};
    s16 hvals[HCOUNT];
    for(u16 i = 0;i<HCOUNT;i++){
        hvals[i] = offset_x + i;
    }
    VDP_setHorizontalScrollLine(BG_B, 8, hvals, 8, DMA);
    VDP_setVerticalScrollTile(BG_B, current_column, vvals, 1, DMA);
}

void line_plane_test_init(){
    VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
}

void line_plane_test_update(){
    s16 hvals[HCOUNT];
    for(u16 i = 0;i<HCOUNT;i++){
        hvals[i] = offset_x + i;
    }
    VDP_setHorizontalScrollLine(BG_B, 8, hvals, 8, DMA);
    VDP_setVerticalScroll(BG_B, offset_y);
}

void tile_test_init(){
    VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_COLUMN);
}

void tile_test_update(){
    s16 vvals[] = {offset_y};
    s16 hvals[] = {offset_x};
    VDP_setHorizontalScrollTile(BG_B, 1, hvals, 1, DMA);
    VDP_setVerticalScrollTile(BG_B, current_column, vvals, 1, DMA);
}

void screen_test_init(){
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_COLUMN);
}

void screen_test_update(){
    s16 vvals[] = {offset_y};
    VDP_setVerticalScrollTile(BG_B, current_column, vvals, 1, DMA);
    VDP_setHorizontalScroll(BG_B, offset_x);
}

void full_test_init(){
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
}

void full_test_update(){
    VDP_setVerticalScroll(BG_B, offset_y);
    VDP_setHorizontalScroll(BG_B, offset_x);
}