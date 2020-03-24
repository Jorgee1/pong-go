#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <string>
#include <stdlib.h> 
#include <time.h>

#include "window.h"
#include "texture.h"
#include "action.h"

bool Verificar_collision(
        SDL_Rect A,
        SDL_Rect B,
        SDL_Point A_VEL,
        SDL_Point* collition
    ){
    /// PARA A
    int A_IZQ = A.x;
    int A_DER = A.x + A.w;
    int A_ARR = A.y;
    int A_ABJ = A.y + A.h;
    /// PARA B
    int B_IZQ = B.x;
    int B_DER = B.x + B.w;
    int B_ARR = B.y;
    int B_ABJ = B.y + B.h;
    /// Restriccones
    if(A_VEL.x>0){
        return false;
    }

    if(
        (A_ABJ >= B_ARR) &&
        (A_ARR <= B_ABJ) &&
        (A_DER >= B_IZQ) &&
        (A_IZQ <= B_DER)
    ){
        if(
            (A_ABJ >= B_ARR) &&
            (A_ABJ <= B_ARR+(B.h/5)) 
        ){
            if(A_VEL.y > 0){
                collition->y = 1;
                collition->x = 1;
            }else{
                collition->x = 1;
            }
            return true;
        }
        if(
            (A_ARR <= B_ABJ) &&
            (A_ARR >= B_ABJ-(B.h/5)) 
        ){
            if(A_VEL.y < 0){
                collition->y = 1;
                collition->x = 1;
            }else{
                collition->x = 1;
            }
            return true;
        }
        if(
            (A_IZQ <= B_DER) &&
            (A_VEL.x < 0) 
        ){
            collition->x = 1;
            return true;
        }

    }
    return false;
}

bool Verificar_collision_IA(
        SDL_Rect A,
        SDL_Rect B,
        SDL_Point A_VEL,
        SDL_Point* collition
    ){
    /// PARA A
    int A_IZQ = A.x;
    int A_DER = A.x + A.w;
    int A_ARR = A.y;
    int A_ABJ = A.y + A.h;
    /// PARA B
    int B_IZQ = B.x;
    int B_DER = B.x + B.w;
    int B_ARR = B.y;
    int B_ABJ = B.y + B.h;
    /// Restriccones
    if(A_VEL.x<0){
        return false;
    }

    if(
        (A_ABJ >= B_ARR) &&
        (A_ARR <= B_ABJ) &&
        (A_DER >= B_IZQ) &&
        (A_IZQ <= B_DER)
    ){
        if(
            (A_ABJ >= B_ARR) &&
            (A_ABJ <= B_ARR+(B.h/5))
        ){
            if(A_VEL.y > 0){
                collition->y = 1;
                collition->x = 1;
            }else{
                collition->x = 1;
            }
            return true;
        }
        if(
            (A_ARR <= B_ABJ) &&
            (A_ARR >= B_ABJ-(B.h/5)) 
        ){
            if(A_VEL.y<0){
                collition->y = 1;
                collition->x = 1;
            }else{
                collition->x = 1;
            }
            return true;
        }
        if(
            (A_DER >= B_IZQ) &&
            (A_VEL.x > 0)
        ){
            collition->x = 1;
            return true;
        }

    }
    return false;
}

class Entity{
    public:
        int x;
        int y;
        int h;
        int w;

        SDL_Point speed_limit;
        SDL_Point speed;
        SDL_Color color;
        
        Entity(){
            x = 0;
            y = 0;
            h = 0;
            w = 0;
            speed.x = 0;
            speed.y = 0;
            speed_limit.x = 0;
            speed_limit.y = 0;
            color = {0, 0, 0, 0xFF};
        }
        
        void init(
            int x, int y, int h, int w,
            SDL_Point speed, SDL_Color color
        ){
            this->x = x;
            this->y = y;
            this->h = h;
            this->w = w;
            this->speed_limit = speed;
            this->color = color;
        }
        
        void move_up(){
            speed.y = -speed_limit.y;
        }
        
        void move_down(){
            speed.y = +speed_limit.y;
        }
        
        void move_left(){
            speed.x = -speed_limit.x;
        }
        
        void move_right(){
            speed.x = +speed_limit.x;
        }
        
        void stop(){
            speed.x = 0;
            speed.y = 0;
        }
        
        void stop_x(){
            speed.x = 0;
        }
        
        void stop_y(){
            speed.y = 0;
        }
        
        void update(){
            x += speed.x;
            y += speed.y;
        }
        
        SDL_Rect get_rect(){
            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.h = h;
            rect.w = w;
            return rect;
        }
        
        SDL_Point get_position(){
            SDL_Point point;
            point.x = x;
            point.y = y;
            return point;
        }
};

int main( int argc, char* args[] ){
    srand (time(NULL));
    int SCREEN_WIDTH  = 640;
    int SCREEN_HEIGHT = 480;
    int TEXT_SIZE     =  40;
    
    int VEL       =  5;
    int BALL_SIZE = 10;

    int PLAYER_WIDHT  =  20;
    int PLAYER_HEIGHT = 100;

    int score[2] = {0,0};

    bool exit = false;

    enum entity{
        BALL,
        PLAYER1,
        PLAYER2
    };

    std::string PATH(SDL_GetBasePath());
    std::string GAME_NAME = "PONG";
    std::string PATH_FONT = PATH + "asset/font/LiberationMono-Regular.ttf";
    std::string PATH_ICON = PATH + "asset/icon.bmp";

    SDL_Color COLOR_BLACK = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_RED   = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_GREEN = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color COLOR_BLUE  = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Color COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};

    Entity ball;
    Entity player1;
    Entity player2;
    
    ball.init(
        SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
        BALL_SIZE, BALL_SIZE, {VEL, VEL/2},
        COLOR_WHITE
    );

    
    ball.move_up();
    ball.move_right();

    player1.init(
        0, 0,
        PLAYER_HEIGHT, PLAYER_WIDHT, {VEL, VEL},
        COLOR_WHITE
    );
    
    player2.init(
        SCREEN_WIDTH - PLAYER_WIDHT, 0,
        PLAYER_HEIGHT, PLAYER_WIDHT, {VEL, VEL},
        COLOR_WHITE
    );
    
    SDL_Point collition;
    
    Window window(
        GAME_NAME.c_str(),
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        COLOR_BLACK
    );
    window.set_icon(PATH_ICON);

    TextureText text_white(
        window.get_render(),
        PATH_FONT,
        COLOR_WHITE,
        TEXT_SIZE
    );

    Action* action = Action::get_instance();
    action->init(
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    );

    while(exit == false){
        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            //Movement Player
            if(action->get_state(action->BUTTON_MOVE_UP)){
                player1.move_up();
            }else if(action->get_state(action->BUTTON_MOVE_DOWN)){
                player1.move_down();
            }else{
                player1.stop();
            }

            if((player1.y <= 0) && (player1.speed.y < 0)){
                player1.stop_y();
                player1.y = 0;
            }else if(
                (player1.y + player1.h >= SCREEN_HEIGHT) &&
                (player1.speed.y > 0)   
            ){
                player1.stop_y();
                player1.y = SCREEN_HEIGHT - player1.h;
            }

            // Movement computer
            if(player2.y + (player2.h/2) + 20 < ball.y){
                player2.move_down();
            }else if(player2.y + (player2.h/2) - 20 > ball.y){
                player2.move_up();
            }else{
                player2.stop();
            }
            
            if(
                (player2.y + player2.h >= SCREEN_HEIGHT) &&
                (player2.speed.y > 0)
            ){
                player2.stop_y();
                player2.y = SCREEN_HEIGHT - player2.h;
            }else if((player2.y <= 0) && (player2.speed.y < 0)){
                player2.stop_y();
                player2.y = 0;
            }

            // ball movement
            
            if(ball.x + ball.w > SCREEN_WIDTH){
                score[PLAYER1 - 1]++;
                ball.x = SCREEN_WIDTH/2;
                ball.y = SCREEN_HEIGHT/2;

                ball.move_up();
            }
            if(ball.x < 0){
                score[PLAYER2 - 1]++;
                ball.x = SCREEN_WIDTH/2;
                ball.y = SCREEN_HEIGHT/2;

                ball.move_right();
            }

            collition.x = 0;
            collition.y = 0;

            // Bounce on screen
            if(ball.y + ball.h > SCREEN_HEIGHT){
                collition.y = 1;
            }
            if(ball.y < 0){
                collition.y = 1;
            }

            // bounce on each elements
            Verificar_collision(
                ball.get_rect(),
                player1.get_rect(),
                ball.speed,
                &collition
            );
            Verificar_collision_IA(
                ball.get_rect(),
                player2.get_rect(),
                ball.speed,
                &collition
            );

            if( (collition.x == 1) || (collition.y == 1)){
                if(collition.x == 1){
                    ball.speed.x = -1*ball.speed.x;
                    collition.x = 0;
                }
                if(collition.y == 1){
                    ball.speed.y = -1*ball.speed.y;
                    collition.y = 0;
                }
            }

            // Update world
            ball.update();
            player1.update();
            player2.update();
            
            // Draw entitys
            window.draw_rectangle(ball.get_rect(), COLOR_WHITE);
            window.draw_rectangle(player1.get_rect(), COLOR_WHITE);
            window.draw_rectangle(player2.get_rect(), COLOR_WHITE);

            // Draw UI
            int width = 0;
            width = text_white.get_text_size(
                std::to_string(score[PLAYER1 - 1])
            ).w;

            text_white.render(
                SCREEN_WIDTH/2 - width -  10, 0,
                std::to_string(score[PLAYER1 - 1])
            );

            width = text_white.get_text_size(
                std::to_string(score[PLAYER2 - 1])
            ).w;

            text_white.render(
                SCREEN_WIDTH/2 + 10, 0,
                std::to_string(score[PLAYER2 - 1])
            );
            
            window.draw_line(
                {SCREEN_WIDTH/2, 0},
                {SCREEN_WIDTH/2, SCREEN_HEIGHT},
                COLOR_WHITE
            );

            window.update_screen();
        }
    }
    return 0;
}
