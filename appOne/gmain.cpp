#define _EX

#ifdef _EX
#include"libOne.h"
struct FILE_ {
    const char* file_title = 0;
    const char* file_play = 0;
    int blockImg = 0;
    int itemblockImg = 0;
    int goalblockImg = 0;
    int fileSize = 0;
    int images_stand = 0;
    const int numImages = 8;
    int image_No = 0;
    int counter = 0;
    int interval = 0;
    //[numImages].
    int images_move[8] = { 0 };
    int images_move_reverse[8] = { 0 };
    char fileName_move[64] = { 0 };
    char fileName_move_reverse[64] = { 0 };
};
struct MAP {
    char* title = 0;
    char* map = 0;
    int title_count = 0;
    int play_count = 0;
    int title_cols = 0;
    int play_cols = 0;
    int title_rows = 0;
    int play_rows = 0;
    int index = 0;
    int start_Cols = 0;
    int title_display_Cols = 0;
    int play_display_Cols = 0;
    int title_end_Cols = 0;
    int play_end_Cols = 0;
};
struct PLAYER {
    float player_x = 0;
    float player_y = 0;
    float player_width = 0;
    float player_height = 0;
    float last_player_x = 0;
    float last_player_y = 0;
    float angle = 0;
    float scale = 0.7f;
    char walk_speed = 5;
    char dash_speed = 8;
    char JUMP = 0;
    bool STOP = 0;
    bool DIRECTION = 0;
    float jump_potition_y = 0;
    const float MAX_JUMP = 250;
};
struct BLOCK {
    float block_x = 0;
    float block_y = 0;
    float size = 100;
    float collision_x = 0;
    float collision_y = 0;
    float goal_x = 0;
};
struct WORLD {
    float worldX = 0;
    float title_endWorldX = 0;
    float play_endWorldX = 0;
};
struct GAME {
    const enum {
       init, title,play,over,clear
    };
    char game_state = init;
    bool death_Flag = 0;
};
void assets_road(struct FILE_* f_pointer) {
    f_pointer->file_title = "assets\\title.txt";
    f_pointer->file_play = "assets\\map.txt";
    f_pointer->blockImg = loadImage("assets\\block.png");
    f_pointer->goalblockImg = loadImage("assets\\goal.png");
    f_pointer->itemblockImg = loadImage("assets\\item.png");
    f_pointer->images_stand = loadImage("assets\\stand.png");
    for (int i = 0; i < f_pointer->numImages; i++) {
        //f_pointer->fileName_move[64];
        //sprintf_s(f_pointer->fileName_move, "assets\\move%d.png", i);
        snprintf(f_pointer->fileName_move, sizeof(f_pointer->fileName_move), "assets\\move%d.png", i);
        f_pointer->images_move[i] = loadImage(f_pointer->fileName_move);
    }
    for (int j = 0; j < f_pointer->numImages; j++) {
        //f_pointer->fileName_move_reverse[64];
        //sprintf_s(f_pointer->fileName_move_reverse, "assets\\move%d_reverse.png", j);
        snprintf(f_pointer->fileName_move_reverse, sizeof(f_pointer->fileName_move_reverse), "assets\\move%d_reverse.png", j);
        f_pointer->images_move_reverse[j] = loadImage(f_pointer->fileName_move_reverse);
    }
}
void TITLE_FILE_OPEN_etc_MEMORY_ALLOCATION(struct FILE_* f_pointer, struct MAP* m_pointer,FILE** fp_pointer) {
    fopen_s(fp_pointer,f_pointer->file_title,"rb");
    fseek(*fp_pointer, 0, SEEK_END);
    f_pointer->fileSize = ftell(*fp_pointer);
    fseek(*fp_pointer, 0, SEEK_SET);
    m_pointer->title = (char*)malloc(f_pointer->fileSize);
    if (m_pointer->title == NULL) {
        //print("false!");
        fclose(*fp_pointer);
        return;
    }
    fread(m_pointer->title, sizeof(char), f_pointer->fileSize, *fp_pointer);
    fclose(*fp_pointer);
    print((let)"fileSize=" + f_pointer->fileSize);
    for (int i = 0; i < f_pointer->fileSize; i++) {
        m_pointer->title_count++;
        if (m_pointer->title[i] == '\n') {
            if (m_pointer->title_rows == 0) {
                m_pointer->title_cols = m_pointer->title_count;
            } 
            else if (m_pointer->title_cols != m_pointer->title_count) {
                WARNING(1, "列数が揃っていない.", "");
            }
            m_pointer->title_rows++;
            m_pointer->title_count = 0;
        }
    }
    if (f_pointer->fileSize % m_pointer->title_cols != 0) {
        WARNING(1, "改行してない.", "");
    }
    //print((let)"rows=" + m_pointer->rows);
    //print((let)"rows=" + m_pointer->cols);
}
void PLAY_FILE_OPEN_etc_MEMORY_ALLOCATION(struct FILE_* f_pointer, struct MAP* m_pointer, FILE** fp_pointer) {
    fopen_s(fp_pointer, f_pointer->file_play, "rb");
    fseek(*fp_pointer, 0, SEEK_END);
    f_pointer->fileSize = ftell(*fp_pointer);
    fseek(*fp_pointer, 0, SEEK_SET);
    m_pointer->map = (char*)malloc(f_pointer->fileSize);
    if (m_pointer->map == NULL) {
        free(m_pointer->title);
        //print("false!");
        fclose(*fp_pointer);
        return;
    }
    fread(m_pointer->map, sizeof(char), f_pointer->fileSize, *fp_pointer);
    fclose(*fp_pointer);
    print((let)"fileSize=" + f_pointer->fileSize);
    for (int i = 0; i < f_pointer->fileSize; i++) {
        m_pointer->play_count++;
        if (m_pointer->map[i] == '\n') {
            if (m_pointer->play_rows == 0) {
                m_pointer->play_cols = m_pointer->play_count;
            }
            else if (m_pointer->play_cols != m_pointer->play_count) {
                WARNING(1, "列数が揃っていない.", "");
            }
            m_pointer->play_rows++;
            m_pointer->play_count = 0;
        }
    }
    if (f_pointer->fileSize % m_pointer->play_cols != 0) {
        WARNING(1, "改行してない.", "");
    }
    //print((let)"rows=" + m_pointer->rows);
    //print((let)"rows=" + m_pointer->cols);
}
void init_for_TITLE(struct MAP* m_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    w_pointer->worldX = 0;
    m_pointer->title_display_Cols = width / b_pointer->size + 1;
    w_pointer->title_endWorldX = b_pointer->size * (m_pointer->title_cols - 2) - width;
}
void init_for_PLAY(struct MAP* m_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    w_pointer->worldX = 0;
    m_pointer->play_display_Cols = width / b_pointer->size + 1;
    w_pointer->play_endWorldX = b_pointer->size * (m_pointer->play_cols - 2) - width;
    p_pointer->player_x = width / 2;
    p_pointer->player_y = height / 2;
    p_pointer->player_width = 100 / 10 * (p_pointer->scale * 10);
    p_pointer->player_height = 190 / 10 * (p_pointer->scale * 10);
}
void INIT(struct GAME* g_pointer,struct PLAYER* p_pointer,struct BLOCK* b_pointer,struct WORLD* w_pointer, struct FILE_* f_pointer, struct MAP* m_pointer, FILE** fp_pointer) {
    assets_road(f_pointer);
    TITLE_FILE_OPEN_etc_MEMORY_ALLOCATION(f_pointer, m_pointer, fp_pointer);
    init_for_TITLE(m_pointer,p_pointer, b_pointer,w_pointer);
    PLAY_FILE_OPEN_etc_MEMORY_ALLOCATION(f_pointer, m_pointer, fp_pointer);
    g_pointer->game_state = g_pointer->title;
}
void TITLE_gimmick(struct GAME* g_pointer, struct MAP* m_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer, struct FILE_* f_pointer, FILE** fp_pointer) {
    fill(255);
    rect(450, 650, 1000, 200);
    fill(0);
    textSize(100);
    text("START Press'SPACE'", 500, 800);
    if (isTrigger(KEY_SPACE)) {
        //free(m_pointer->title);
        //m_pointer->title = NULL;
        //PLAY_FILE_OPEN_etc_MEMORY_ALLOCATION(f_pointer, m_pointer, fp_pointer);
        init_for_PLAY(m_pointer, p_pointer, b_pointer, w_pointer);
        g_pointer->game_state = g_pointer->play;
    }
}
void TITLE_MAP_DRAW(struct FILE_* f_pointer, struct MAP* m_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    noStroke();
    fill(60, 120, 255);
    rect(0, 0, width, height);
    m_pointer->start_Cols = w_pointer->worldX / b_pointer->size;
    m_pointer->title_end_Cols = m_pointer->start_Cols + m_pointer->title_display_Cols;
    for (int c = m_pointer->start_Cols; c < m_pointer->title_end_Cols; c++) {
        b_pointer->block_x = -w_pointer->worldX + b_pointer->size * c;
        for (int r = 0; r < m_pointer->title_rows; r++) {
            b_pointer->block_y = b_pointer->size * r;
            m_pointer->index = r * m_pointer->title_cols + c;
            if (m_pointer->title[m_pointer->index] == 'b') {
                image(f_pointer->blockImg, b_pointer->block_x, b_pointer->block_y);
            }
            else if (m_pointer->title[m_pointer->index] == 'i') {
                image(f_pointer->itemblockImg, b_pointer->block_x, b_pointer->block_y);
            }
        }
    }
}
void PLAY_MAP_DRAW(struct FILE_* f_pointer,struct MAP* m_pointer,struct PLAYER* p_pointer,struct BLOCK* b_pointer,struct WORLD* w_pointer) {
    noStroke();
    fill(60, 120, 255);
    rect(0, 0, width, height);
    m_pointer->start_Cols = w_pointer->worldX / b_pointer->size;
    m_pointer->play_end_Cols = m_pointer->start_Cols + m_pointer->play_display_Cols;
    for (int c = m_pointer->start_Cols; c < m_pointer->play_end_Cols; c++) {
        b_pointer->block_x = -w_pointer->worldX + b_pointer->size * c;
        for (int r = 0; r < m_pointer->play_rows; r++) {
            b_pointer->block_y = b_pointer->size * r;
            m_pointer->index = r * m_pointer->play_cols + c;
            if (m_pointer->map[m_pointer->index] == 'b') {
                image(f_pointer->blockImg, b_pointer->block_x, b_pointer->block_y);
            }
            else if (m_pointer->map[m_pointer->index] == 'i') {
                image(f_pointer->itemblockImg, b_pointer->block_x, b_pointer->block_y);
            }
            else if (m_pointer->map[m_pointer->index] == 'g') {
                image(f_pointer->goalblockImg, b_pointer->block_x, b_pointer->block_y);
            }
        }
    }
}
void TITLE_WORLD_SCROLL(struct GAME* g_pointer, struct WORLD* w_pointer, struct PLAYER* p_pointer) {
    if (w_pointer->worldX >= w_pointer->title_endWorldX) {
        w_pointer->worldX = w_pointer->title_endWorldX;
    }
    else {
        p_pointer->player_x -= 5;
        w_pointer->worldX += 5;
    }
}
void PLAY_WORLD_SCROLL(struct GAME* g_pointer, struct WORLD* w_pointer, struct PLAYER* p_pointer) {
    if (w_pointer->worldX >= w_pointer->play_endWorldX) {
        w_pointer->worldX = w_pointer->play_endWorldX;
    }
    else {
        p_pointer->player_x -=3;
        w_pointer->worldX += 3;
    }
}

void PLAYER_DRAW(struct FILE_* f_pointer, struct PLAYER* p_pointer) {
    if (p_pointer->DIRECTION == 0 && p_pointer->STOP == 1) {
        image(f_pointer->images_move_reverse[f_pointer->image_No], p_pointer->player_x, p_pointer->player_y, p_pointer->angle, p_pointer->scale);
    }
    else if (p_pointer->DIRECTION == 1 && p_pointer->STOP == 1) {
        image(f_pointer->images_move[f_pointer->image_No], p_pointer->player_x, p_pointer->player_y, p_pointer->angle, p_pointer->scale);
    }
    if (p_pointer->STOP == 0) {
        image(f_pointer->images_stand, p_pointer->player_x, p_pointer->player_y, p_pointer->angle, p_pointer->scale);
    }
}
void PLAYER_MOVE(struct FILE_* f_pointer, struct PLAYER* p_pointer) {
    if (isPress(KEY_W)) {
        f_pointer->interval = 3;
    }
    else {
        f_pointer->interval = 6;
    }
    ++f_pointer->counter %= f_pointer->numImages * f_pointer->interval;
    f_pointer->image_No = f_pointer->counter / f_pointer->interval;
    if (isPress(KEY_W)) {
        if (isPress(KEY_D)) { 
            p_pointer->player_x += p_pointer->dash_speed;
            p_pointer->STOP = 1;
            p_pointer->DIRECTION = 0;
        }
        if (isPress(KEY_A)) { 
            p_pointer->player_x -= p_pointer->dash_speed;
            p_pointer->STOP = 1;
            p_pointer->DIRECTION = 1;
        }
        else if (!isPress(KEY_D)&&!isPress(KEY_A)){
            p_pointer->STOP = 0;
        }
    }
    else { 
        if (isPress(KEY_D)) {
            p_pointer->player_x += p_pointer->walk_speed;
            p_pointer->STOP = 1;
            p_pointer->DIRECTION = 0;
        }
        if (isPress(KEY_A)) {
            p_pointer->player_x -= p_pointer->walk_speed;
            p_pointer->STOP = 1;
            p_pointer->DIRECTION = 1;
        }
        else if (!isPress(KEY_D) && !isPress(KEY_A)) {
            p_pointer->STOP = 0;
        }
    }
}
void collision(struct MAP* m_pointer, struct FILE_* f_pointer, struct BLOCK* b_pointer, struct PLAYER* p_pointer, struct WORLD* w_pointer) {
    /*
    noStroke();
    fill(60, 120, 255);
    rect(0, 0, width, height);
    fill(0);
    */
    m_pointer->start_Cols = w_pointer->worldX / b_pointer->size;
    m_pointer->play_end_Cols = m_pointer->start_Cols + m_pointer->play_display_Cols;
    for (int c = m_pointer->start_Cols; c < m_pointer->play_end_Cols; c++) {
        b_pointer->block_x = -w_pointer->worldX + b_pointer->size * c;
        for (int r = 0; r < m_pointer->play_rows; r++) {
            //b_pointer->block_y = b_pointer->size * r;
            m_pointer->index = r * m_pointer->play_cols + c;
            b_pointer->collision_x = 0;
            b_pointer->collision_y = 0;
            if (m_pointer->map[m_pointer->index] == 'b') {
                //rect(b_pointer->block_x, b_pointer->block_y, 108, 108);
                b_pointer->collision_x = -w_pointer->worldX + b_pointer->size * c;
                b_pointer->collision_y = b_pointer->size * r;
            }
            else if (m_pointer->map[m_pointer->index] == 'i') {
                //rect(b_pointer->block_x, b_pointer->block_y, 108, 108);
                b_pointer->collision_x = -w_pointer->worldX + b_pointer->size * c;
                b_pointer->collision_y = b_pointer->size * r;
            }
            else if (m_pointer->map[m_pointer->index] == 'g') {
                //rect(b_pointer->block_x, b_pointer->block_y, 108, 108);
                b_pointer->collision_x = -w_pointer->worldX + b_pointer->size * c;
                b_pointer->collision_y = b_pointer->size * r;
            }
            if (p_pointer->player_x + p_pointer->player_width > b_pointer->collision_x &&p_pointer->player_x < b_pointer->collision_x + b_pointer->size) {
                if (p_pointer->player_y + p_pointer->player_height > b_pointer->collision_y &&p_pointer->player_y < b_pointer->collision_y + b_pointer->size) {

                    // もしプレイヤーが「下から」ブロックにめり込んでいる場合は、Y軸を修正
                    if (p_pointer->player_y + p_pointer->player_height <= b_pointer->collision_y) {
                        p_pointer->player_y = b_pointer->collision_y - p_pointer->player_height;  // 床の上に乗せる
                        p_pointer->JUMP = 2;
                    }
                    else {
                        p_pointer->player_y = p_pointer->last_player_y;  // その他のY軸衝突
                        p_pointer->JUMP = 0;
                    }

                    // X軸の衝突判定を追加
                    if (p_pointer->last_player_x + p_pointer->player_width <= b_pointer->collision_x) {
                        p_pointer->player_x = b_pointer->collision_x - p_pointer->player_width;// 左から壁に当たった.
                    }
                    if (p_pointer->last_player_x >= b_pointer->collision_x + b_pointer->size) {
                        if (b_pointer->collision_y<p_pointer->player_y+(p_pointer->player_height)) {
                        p_pointer->player_x = b_pointer->collision_x + b_pointer->size; // 右から壁に当たった.
                        }
                    }
                }
            }
            if ((p_pointer->player_x + p_pointer->player_width)+5 > width) {
                p_pointer->player_x -= 1;// 左から壁に当たった.
            }
        }
    }
}
void remain_player_potition(struct PLAYER* p_pointer) {
    p_pointer->last_player_x = p_pointer->player_x;
    p_pointer->last_player_y = p_pointer->player_y;
}
void JUMP_and_FALL(struct PLAYER* p_pointer) {
    if (p_pointer->JUMP == 0) {
        if (isTrigger(KEY_SPACE)) {
            p_pointer->jump_potition_y = p_pointer->player_y - p_pointer->MAX_JUMP;
            p_pointer->JUMP = 1;
        }
        else {
            p_pointer->player_y += 12;
        }
    }
    else if (p_pointer->JUMP == 1) {
        if (p_pointer->jump_potition_y <= p_pointer->player_y) {
            p_pointer->player_y -= 12;
        }
        else {
            p_pointer->JUMP = 2;
        }
    }
    else if (p_pointer->JUMP == 2) {
        p_pointer->player_y += 12;
    }
}
void Damage_gudgment(struct GAME* g_pointer, struct PLAYER* p_pointer) {
    if ((p_pointer->player_x + p_pointer->player_width) <= 0 || p_pointer->player_y >= height) {
        g_pointer->game_state = g_pointer->over;
    }
}
void Clear_gudgment(struct GAME* g_pointer, struct PLAYER* p_pointer, struct MAP* m_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    m_pointer->start_Cols = w_pointer->worldX / b_pointer->size;
    m_pointer->play_end_Cols = m_pointer->start_Cols + m_pointer->play_display_Cols;
    for (int c = m_pointer->start_Cols; c < m_pointer->play_end_Cols; c++) {
        b_pointer->block_x = -w_pointer->worldX + b_pointer->size * c;
        for (int r = 0; r < m_pointer->play_rows; r++) {
            m_pointer->index = r * m_pointer->play_cols + c;
            b_pointer->goal_x = 0;
            if (m_pointer->map[m_pointer->index] == 'g') {
                b_pointer->goal_x = -w_pointer->worldX + b_pointer->size * c;
            }
            if (b_pointer->goal_x >= 10) {
                if (p_pointer->player_x > b_pointer->goal_x) {
                    g_pointer->game_state = g_pointer->clear;
                }
            }
        }
    }
}
void GAME_OVER_Gimmick(struct GAME* g_pointer, struct MAP* m_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    fill(0);
    textSize(200);
    text("YOU DEAD!!", 0, 200);
    textSize(100);
    text("Restart", 50, 500);
    text("Press SPACE", 50, 600);
    if (isTrigger(KEY_SPACE)) {
        init_for_PLAY(m_pointer, p_pointer, b_pointer, w_pointer);
        g_pointer->game_state = g_pointer->play;
    }
}
void GAME_CLEAR_Gimmick(struct GAME* g_pointer, struct MAP* m_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    fill(0);
    textSize(200);
    text("YOU SUCCESS!!", 0, 200);
    textSize(100);
    text("Restart", 50, 500);
    text("Press SPACE", 50, 600);
    if (isTrigger(KEY_SPACE)) {
        init_for_TITLE(m_pointer, p_pointer, b_pointer, w_pointer);
        g_pointer->game_state = g_pointer->title;
    }
}
void TITLE(struct GAME* g_pointer,struct FILE_* f_pointer,struct MAP* m_pointer, FILE** fp_pointer,struct PLAYER* p_pointer,struct BLOCK* b_pointer,struct WORLD* w_pointer ) {
    clear(0);
    TITLE_WORLD_SCROLL(g_pointer,w_pointer, p_pointer);
    TITLE_MAP_DRAW(f_pointer, m_pointer, p_pointer, b_pointer, w_pointer);
    TITLE_gimmick(g_pointer,m_pointer,p_pointer,b_pointer,w_pointer,f_pointer,fp_pointer);
}
void PLAY(struct GAME* g_pointer, struct FILE_* f_pointer, struct MAP* m_pointer, FILE** fp_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    clear(0);
    Damage_gudgment(g_pointer, p_pointer);
    Clear_gudgment(g_pointer, p_pointer,m_pointer,b_pointer,w_pointer);
    PLAY_WORLD_SCROLL(g_pointer,w_pointer,p_pointer);
    remain_player_potition(p_pointer);
    JUMP_and_FALL(p_pointer);
    PLAYER_MOVE(f_pointer, p_pointer);
    PLAY_MAP_DRAW(f_pointer, m_pointer, p_pointer, b_pointer,w_pointer);
    collision(m_pointer, f_pointer, b_pointer, p_pointer, w_pointer);
    PLAYER_DRAW(f_pointer, p_pointer);
}
void OVER(struct GAME* g_pointer, struct FILE_* f_pointer, struct MAP* m_pointer, FILE** fp_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    clear(0);
    PLAY_MAP_DRAW(f_pointer, m_pointer, p_pointer, b_pointer, w_pointer);
    GAME_OVER_Gimmick(g_pointer, m_pointer, p_pointer, b_pointer, w_pointer);
}
void CLEAR(struct GAME* g_pointer, struct FILE_* f_pointer, struct MAP* m_pointer, FILE** fp_pointer, struct PLAYER* p_pointer, struct BLOCK* b_pointer, struct WORLD* w_pointer) {
    clear(0);
    PLAY_MAP_DRAW(f_pointer, m_pointer, p_pointer, b_pointer, w_pointer);
    GAME_CLEAR_Gimmick(g_pointer, m_pointer, p_pointer, b_pointer, w_pointer);
}
void gmain() {
    window(1900, 1000, full);
    FILE* fp = 0;
    struct FILE_ f;
    struct MAP m;
    struct PLAYER p;
    struct BLOCK b;
    struct WORLD w;
    struct GAME g;
    while (notQuit) {
        if (g.game_state == g.init) { INIT(&g, &p, &b, &w, &f, &m, &fp); }
        else if (g.game_state == g.title) { TITLE(&g, &f, &m, &fp, &p, &b, &w); }
        else if (g.game_state == g.play) { PLAY(&g, &f, &m, &fp, &p, &b, &w); }
        else if (g.game_state == g.over) { OVER(&g, &f, &m, &fp, &p, &b, &w); }
        else if (g.game_state == g.clear) { CLEAR(&g, &f, &m, &fp, &p, &b, &w); }
    }
    free(m.map);
    free(m.title);
}
#endif

#ifdef _SAMPLE
#include"libOne.h"
void gmain() {
    window(1900, 1000, full);
    //画像読み込み
    int blockImg = loadImage("assets\\block.png");
    int itemBlockImg = loadImage("assets\\item.png");
    //ファイルを開く
    const char* fileName = "assets\\map.txt";
    FILE* fp = 0;
    fopen_s(&fp, fileName, "rb");//"rb"→read binary
    WARNING(fp == 0, fileName, "ファイル読み込みエラー");
    //ファイルサイズ取得
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    //読み込む配列を確保
    char* map = (char*)malloc(fileSize);
    WARNING(map == 0, "メモリ確保エラー", "");
    //データを読み込む
    fread(map, sizeof(char), fileSize, fp);
    fclose(fp);
    //行数、列数を数える（最後の行も必ず改行して終わっていることが条件）
    int rows = 0;
    int cols = 0;//改行２文字分を含んだ列数になる
    int cnt = 0;
    for (int i = 0; i < fileSize; i++) {
        cnt++;
        //行の最後の文字
        if (map[i] == '\n') {
            if (rows == 0) {
                //最初の行の列数
                cols = cnt;
            }
            else if (cols != cnt) {
                //行ごとの列数が違ったらエラー
                WARNING(1, "列数が不揃い", "");
            }
            //行を数えてカウンタをリセット
            rows++;
            cnt = 0;
        }
    }
    if (fileSize % cols != 0) {
        WARNING(1, "最後の行を改行していない", "");
    }
    //マップスクロール・表示用データ
    int size = 100;//１つのブロックサイズ
    float worldX = 0;//現在表示しているワールド座標
    int dispCols = (int)width / size + 1;//表示すべき列数
    float worldWidth = size * (cols - 2);//ワールドの横幅
    float endWorldX = worldWidth - width;//表示できる最後の座標
    //メインループ
    hideCursor();
    while (notQuit) {
        //スクロール
        worldX += 5;
        if (worldX > endWorldX)worldX = endWorldX;
        //描画-----------------------------------------------------------
        clear();
        //　空色背景
        noStroke();
        fill(60, 120, 240);
        rect(0, 0, width, height);
        //　マップ
        int startCol = (int)worldX / size;//表示開始列
        int endCol = startCol + dispCols;//表示終了列
        for (int c = startCol; c < endCol; c++) {
            float px = -worldX + size * c;
            for (int r = 0; r < rows; r++) {
                float py = size * r;
                switch (map[r * cols + c]) {
                case 'b':
                    image(blockImg, px, py);
                    break;
                case 'i':
                    image(itemBlockImg, px, py);
                    break;
                default:
                    break;
                }
            }
        }
    }
    //マップデータ開放
    free(map);
}
#endif