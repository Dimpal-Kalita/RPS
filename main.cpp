#include "Game.cpp"

int main(){

    Game game;
    // Game loop
    while(game.running()){
        
        //Update
        game.update();


        //Render
        game.render();
    }

    return 0;
}