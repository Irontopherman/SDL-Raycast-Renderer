#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
static SDL_Window *window = NULL;
static SDL_Renderer *ren = NULL;
float width=1;
float height=1;
int tempw=0;
int temph=0;
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetHint("SDL_RENDER_LINE_METHOD", "2");
    if (!SDL_CreateWindowAndRenderer("Hello World", 900, 900, false, &window, &ren)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_GetWindowSize(window, &tempw, &temph);
    width=tempw;
    height=temph;
    return SDL_APP_CONTINUE;
}
#define mapwidth 10
#define mapheight 10
int map[mapwidth][mapheight] = {
{2,2,2,2,2,2,2,2,2,2},
{2,0,0,0,0,0,0,0,0,2},
{2,0,1,1,1,1,1,1,0,2},
{2,0,1,0,0,0,1,0,1,2},
{2,0,1,0,1,0,0,0,1,2},
{2,0,1,1,1,1,1,1,1,2},
{2,0,1,0,0,0,0,1,0,2},
{2,0,1,0,1,0,1,1,0,2},
{2,0,0,0,1,0,0,0,0,3},
{2,2,2,2,2,2,2,2,3,3},};
SDL_FPoint playerpos= {100.f, 100.f};
const double raycount=900.f;
float raylen=1000;
float samplerate=200;
float playerangle=0;
void raycast(){
    static float endX;
    static float endY;
    static float beginX;
    static float beginY;
    static float pointX;
    static float pointY;
    static float currraylen;
    static int arrX;
    static int arrY;
    static float angleX=0;
    static float angleY=0;
    static float angle=0;
    static float linelen=0;
    static bool linehit=false;
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(ren, 100, 100, 115, 45);
    for (int allrays=raycount;allrays>=0;allrays--){
        //angle=SDL_fmodf(((float)allrays/(float)raycount), 1.f);
        angle=(float)allrays/(float)raycount;

        angleX=SDL_cosf(angle - ((playerangle+30) * (SDL_PI_F/180.f)));
        angleY=SDL_sinf(angle - ((playerangle+30) * (SDL_PI_F/180.f)));
        for (int linepts=0;linepts<=samplerate;linepts++){
            beginX =playerpos.x;
            beginY =playerpos.y; 
            endX = (playerpos.x+(angleX*(linepts*5)));
            endY = (playerpos.y+(angleY*(linepts*5)));
            arrX= endX/(width/mapwidth);
            arrY= endY/(height/mapheight);
            linelen++;
            if (map[arrX][arrY]>0&&(arrX<mapwidth&&arrY<mapheight)){
                linehit=true;
                linelen++;
                break;
            }
            if (arrX>mapwidth||arrY>mapheight)break;
            if (arrX<0||arrY<0)break;
        }
        if (linehit){
            float alpha=(samplerate-linelen*1.25F);
            if (alpha<0)alpha=1;
            switch (map[arrX][arrY]){
                case (1):
                    SDL_SetRenderDrawColor(ren, 125, 125, 125, alpha);
                    break;
                case(2):
                    SDL_SetRenderDrawColor(ren, 25, 100, 145, alpha);
                    break;
                case(3):
                    SDL_SetRenderDrawColor(ren, 10, 145, 65, alpha);
                    break;
            }
            double lineX = (((double)allrays/(double)raycount))*(width);
            //float lineY = (width/2.f)-(float)(((float)(linelen*1.05F)- 2.f)/ 245)*((float)height+25.f)+0.f;
            //double lineY = (width/2.f)-((((float)linelen/(180.f+(linelen)))*(height))-((linelen)/(samplerate*(linelen))));
            //(width/2.f)-((((float)linelen/(180.f+(linelen)))*(height))-((linelen)/(samplerate*(linelen))))
            double flatlinefix = (samplerate/linelen*90.F);
            flatlinefix = flatlinefix/(linelen/10.f);
            flatlinefix = flatlinefix/2.F;
            if (linelen<10)flatlinefix = flatlinefix+(SDL_fabs((width/2)-lineX)/10.F);
            double lineY = (linelen/(180.f+(linelen)));
            lineY = lineY*(height-linelen);
            lineY = lineY-flatlinefix;
            lineY = lineY-(linelen/samplerate);
            lineY = (width/2.f)-lineY;
            lineY = lineY/1.8F;
            if (lineY<0)lineY=0;
            double finalY1 = (height/2.f)+(lineY);
            double finalY2 = (height/2.f)-(lineY);
            SDL_RenderLine(ren, lineX, finalY1, lineX, finalY2);
        }
        SDL_SetRenderDrawColor(ren, 100, 100, 115, 45);
        //SDL_RenderLine(ren, beginX, beginY, endX, endY);
        linehit=false;
        linelen=0;
    }

}
float mouseX;
float oldmausX, oldmausY;
void mouselogic(){
    static float prevX=0;
    static float sensitivety=4.F;
    static float rel_x, rel_y;
    SDL_HideCursor();
    SDL_SetWindowRelativeMouseMode(window, true);
    SDL_PumpEvents();
    SDL_GetRelativeMouseState(&rel_x, &rel_y);
    oldmausX += (double)rel_x;
    oldmausY += (double)rel_y;
    playerangle = -((oldmausX) / sensitivety);
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  
    }
    if (event->type==SDL_EVENT_MOUSE_MOTION){
        //mouselogic();
    }
    return SDL_APP_CONTINUE;
}
bool winkill=false;
void input(){
    static float speed=5;
    static float turnspeed=2.5;
    static int gridX;
    static int gridY;
    static int gridXback;
    static int gridYback;
    gridY=(int)((playerpos.y-(SDL_sinf(playerangle * (SDL_PI_F/180.f))*(speed*2.f)))/(height/mapheight));
    gridX=(int)((playerpos.x+(SDL_cosf(playerangle * (SDL_PI_F/180.f))*(speed*2.f)))/(width/mapwidth));
    gridYback=(int)((playerpos.y+(SDL_sinf(playerangle * (SDL_PI_F/180.f))*(speed*2.f)))/(height/mapheight));
    gridXback=(int)((playerpos.x-(SDL_cosf(playerangle * (SDL_PI_F/180.f))*(speed*2.f)))/(width/mapwidth));
    const bool* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE])winkill=true;
    if (keys[SDL_SCANCODE_UP]||keys[SDL_SCANCODE_W]){
        if (map[gridX][gridY]<1){
            if (gridX<1||gridY<1)return;
            playerpos.y-=(SDL_sinf(playerangle * (SDL_PI_F/180.f))*speed);
            playerpos.x+=(SDL_cosf(playerangle * (SDL_PI_F/180.f))*speed);
        }
    }
    if (keys[SDL_SCANCODE_DOWN]||keys[SDL_SCANCODE_S]){
        if (map[gridXback][gridYback]<1){
            if (gridXback<1||gridYback<1)return;
            playerpos.y+=(SDL_sinf(playerangle * (SDL_PI_F/180.f))*speed);
            playerpos.x-=(SDL_cosf(playerangle * (SDL_PI_F/180.f))*speed);
        }
    }
    if (keys[SDL_SCANCODE_LEFT]){
        playerangle+=turnspeed;
    }
    if (keys[SDL_SCANCODE_RIGHT]){
        playerangle-=turnspeed;
    }
}
SDL_AppResult SDL_AppIterate(void *appstate)
{
    static int64_t begintime=1;
    static int64_t endtime=1;
    static int64_t frametime=1;
    begintime=SDL_GetTicks();
    SDL_SetRenderDrawColor(ren, 25, 25, 25, 255);
    SDL_RenderClear(ren);
    raycast();
    input();
    endtime=SDL_GetTicks();
    SDL_SetRenderDrawColor(ren, 25, 25, 25, 255);
    SDL_RenderPresent(ren);
    frametime=begintime-endtime;
    if (winkill)return SDL_APP_SUCCESS;
    if (frametime<16)SDL_Delay(16-frametime);

    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
