#include <iostream>
#include <sstream>
#include "algorithm"
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "sprite.h"
#include "player.h"
#include "imposter.h"
#include "button.h"
#include "maze.h"
#include "wall.h"
#include "obstacle.h"
#include "text_renderer.h"

// Game-related State data
SpriteRenderer *renderer;
Player *player;
Imposter *imposter;
Button *impButton;
Button *coinButton;
TextRenderer *Text;
Sprite *OverScreen;
Sprite *WinScreen;
Sprite *LoseScreen;
int buttons = 2;
int lastSecondPassed;
bool light = true;
Maze *maze;
std::vector<Wall *> walls;
std::vector<Obstacle *> obstacles;
std::string direction;
std::vector<std::pair<int, int>> grassPlots;
std::vector<std::pair<int, int>> obsGrassPlots;

Game::Game(unsigned int width, unsigned int height)
        : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {
}

Game::~Game() {
    delete player;
    delete imposter;
    delete impButton;
    delete coinButton;
    delete Text;
    delete OverScreen;
    delete WinScreen;
    delete LoseScreen;
    delete maze;
    delete renderer;
}

void Game::Init() {
    // load shaders
    ResourceManager::LoadShader("../source/shaders/sprite.vs", "../source/shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    auto p = ResourceManager::GetShader("sprite");
    renderer = new SpriteRenderer(p);

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../assets/fonts/prata/Prata-Regular.ttf", 24);

    // load textures
    ResourceManager::LoadTexture("../assets/textures/brick.png", true, "brick");
    ResourceManager::LoadTexture("../assets/textures/grass.png", true, "grass");
    ResourceManager::LoadTexture("../assets/textures/bluetile.png", true, "bluetile");

    ResourceManager::LoadTexture("../assets/textures/blue/right0.png", true, "playerr00");
    ResourceManager::LoadTexture("../assets/textures/blue/right1.png", true, "playerr01");
    ResourceManager::LoadTexture("../assets/textures/blue/right2.png", true, "playerr02");
    ResourceManager::LoadTexture("../assets/textures/blue/right3.png", true, "playerr03");
    ResourceManager::LoadTexture("../assets/textures/blue/right4.png", true, "playerr04");
    ResourceManager::LoadTexture("../assets/textures/blue/right5.png", true, "playerr05");
    ResourceManager::LoadTexture("../assets/textures/blue/right6.png", true, "playerr06");
    ResourceManager::LoadTexture("../assets/textures/blue/right7.png", true, "playerr07");
    ResourceManager::LoadTexture("../assets/textures/blue/right8.png", true, "playerr08");
    ResourceManager::LoadTexture("../assets/textures/blue/right9.png", true, "playerr09");
    ResourceManager::LoadTexture("../assets/textures/blue/right10.png", true, "playerr10");
    ResourceManager::LoadTexture("../assets/textures/blue/right11.png", true, "playerr11");
    ResourceManager::LoadTexture("../assets/textures/blue/right12.png", true, "playerr12");
    ResourceManager::LoadTexture("../assets/textures/blue/left0.png", true, "playerl00");
    ResourceManager::LoadTexture("../assets/textures/blue/left1.png", true, "playerl01");
    ResourceManager::LoadTexture("../assets/textures/blue/left2.png", true, "playerl02");
    ResourceManager::LoadTexture("../assets/textures/blue/left3.png", true, "playerl03");
    ResourceManager::LoadTexture("../assets/textures/blue/left4.png", true, "playerl04");
    ResourceManager::LoadTexture("../assets/textures/blue/left5.png", true, "playerl05");
    ResourceManager::LoadTexture("../assets/textures/blue/left6.png", true, "playerl06");
    ResourceManager::LoadTexture("../assets/textures/blue/left7.png", true, "playerl07");
    ResourceManager::LoadTexture("../assets/textures/blue/left8.png", true, "playerl08");
    ResourceManager::LoadTexture("../assets/textures/blue/left9.png", true, "playerl09");
    ResourceManager::LoadTexture("../assets/textures/blue/left10.png", true, "playerl10");
    ResourceManager::LoadTexture("../assets/textures/blue/left11.png", true, "playerl11");
    ResourceManager::LoadTexture("../assets/textures/blue/left12.png", true, "playerl12");

    ResourceManager::LoadTexture("../assets/textures/brown/idleleft.png", true, "impostorl00");
    ResourceManager::LoadTexture("../assets/textures/brown/idleright.png", true, "impostorr00");

    for (int i = 0; i < 24; i++) {
        if (i < 9) {
            ResourceManager::LoadTexture("../assets/textures/brown/right/tile00" + std::to_string(i) + ".png", true,
                                         "impostorr0" + std::to_string(i + 1));
            ResourceManager::LoadTexture("../assets/textures/brown/left/tile00" + std::to_string(i) + ".png", true,
                                         "impostorl0" + std::to_string(i + 1));
        } else if (i == 9) {
            ResourceManager::LoadTexture("../assets/textures/brown/right/tile00" + std::to_string(i) + ".png", true,
                                         "impostorr" + std::to_string(i + 1));
            ResourceManager::LoadTexture("../assets/textures/brown/left/tile00" + std::to_string(i) + ".png", true,
                                         "impostorl" + std::to_string(i + 1));
        } else {
            ResourceManager::LoadTexture("../assets/textures/brown/right/tile0" + std::to_string(i) + ".png", true,
                                         "impostorr" + std::to_string(i + 1));
            ResourceManager::LoadTexture("../assets/textures/brown/left/tile0" + std::to_string(i) + ".png", true,
                                         "impostorl" + std::to_string(i + 1));
        }
    }

    ResourceManager::LoadTexture("../assets/textures/rbut.png", true, "redbutton");
    ResourceManager::LoadTexture("../assets/textures/gbut.png", true, "greenbutton");
    ResourceManager::LoadTexture("../assets/textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("../assets/textures/trap.png", true, "trap");

    ResourceManager::LoadTexture("../assets/textures/go1.png", true, "over");
    ResourceManager::LoadTexture("../assets/textures/gl1.png", true, "lose");
    ResourceManager::LoadTexture("../assets/textures/gw1.png", true, "win");

    OverScreen = new Sprite("over", glm::vec2(this->Width / 2 - 300, this->Height / 2 - 300), glm::vec2(600.0f, 600.0f),
                            0.0f,
                            glm::vec3(1.0f, 1.0f, 1.0f));
    LoseScreen = new Sprite("lose", glm::vec2(this->Width / 2 - 300, this->Height / 2 - 300), glm::vec2(600.0f, 600.0f),
                            0.0f,
                            glm::vec3(1.0f, 1.0f, 1.0f));
    WinScreen = new Sprite("win", glm::vec2(this->Width / 2 - 300, this->Height / 2 - 300), glm::vec2(600.0f, 600.0f),
                           0.0f,
                           glm::vec3(1.0f, 1.0f, 1.0f));

    maze = new Maze(11, 15);

    for (int r = 0; r < maze->rows; r++) {
        for (int c = 0; c < maze->columns; c++) {
            int x = 30 + 50 * c, y = 20 + 50 * r;
            if (maze->maze[r][c] == '#') {
                Wall *tempWall = new Wall("brick", glm::vec2(x, y), glm::vec2(50, 50), true, 0.0f,
                                          glm::vec3(0.5f, 0.5f, 0.5f));
                walls.push_back(tempWall);
            }
            if (maze->maze[r][c] == ' ' || maze->maze[r][c] == 'S' || maze->maze[r][c] == 'E') {
                Wall *tempWall;
                if (maze->maze[r][c] == ' ') {
                    tempWall = new Wall("grass", glm::vec2(x, y), glm::vec2(50, 50), false, 0.0f,
                                        glm::vec3(0.5f, 0.5f, 0.5f));
                } else if (maze->maze[r][c] == 'S') {
                    tempWall = new Wall("bluetile", glm::vec2(x, y), glm::vec2(50, 50), false, 0.0f,
                                        glm::vec3(0.5f, 0.5f, 0.5f));
                } else if (maze->maze[r][c] == 'E') {
                    tempWall = new Wall("brick", glm::vec2(x, y), glm::vec2(50, 50), true, 0.0f,
                                        glm::vec3(0.5f, 0.5f, 0.5f));
                }
                walls.push_back(tempWall);
                if (maze->maze[r][c] == ' ' and r > 4 and c > 6) {
                    grassPlots.push_back(std::make_pair(x, y));
                }
                if (maze->maze[r][c] == ' ') {
                    obsGrassPlots.push_back(std::make_pair(x, y));
                }
            }
        }
    }
    player = new Player("playerr00", glm::vec2(90.0f, 20.0f), glm::vec2(28.0f, 36.0f), 0.0f,
                        glm::vec3(0.5f, 0.5f, 0.5f), 20);


    std::pair<int, int> impPos = grassPlots[rand() % grassPlots.size()];
    imposter = new Imposter("impostorl00",
                            glm::vec2(impPos.first + 25 - 12, impPos.second + 25 - 20),
                            glm::vec2(24.0f, 40.0f), 0.0f,
                            glm::vec3(1.0f, 1.0f, 1.0f));

    std::pair<int, int> impButPos = obsGrassPlots[rand() % obsGrassPlots.size()];
    obsGrassPlots.erase(std::remove(obsGrassPlots.begin(), obsGrassPlots.end(), impButPos), obsGrassPlots.end());
    grassPlots.erase(std::remove(grassPlots.begin(), grassPlots.end(), impButPos), grassPlots.end());
    impButton = new Button("redbutton",
                           glm::vec2(impButPos.first + 25 - 20, impButPos.second + 25 - 20),
                           glm::vec2(40.0f, 40.0f), 0.0f,
                           glm::vec3(1.0f, 1.0f, 1.0f));

    std::pair<int, int> coinButPos = obsGrassPlots[rand() % obsGrassPlots.size()];
    obsGrassPlots.erase(std::remove(obsGrassPlots.begin(), obsGrassPlots.end(), coinButPos), obsGrassPlots.end());
    coinButton = new Button("greenbutton",
                            glm::vec2(coinButPos.first + 25 - 20, coinButPos.second + 25 - 20),
                            glm::vec2(40.0f, 40.0f), 0.0f,
                            glm::vec3(1.0f, 1.0f, 1.0f));

    lastSecondPassed = int(glfwGetTime());
}

void Game::Update(GLFWwindow *window, float dt) {

    if (this->State == GAME_ACTIVE) {
        float velocity = 1;
        // move imposter around
        if (imposter->active) {
            glm::vec2 targetGrassCenter = imposter->position + imposter->size * 0.5f;
            if (targetGrassCenter == imposter->target) {
                std::string direc = maze->computePath(player, imposter);
                if (direc != "continue")
                    direction = direc;
            } else {
                if (imposter->wallCollisions(walls, velocity, direction) == 0) {
                    imposter->move(velocity, this->Width, this->Height, direction);
                } else {
                    imposter->textureString = "imposter" + imposter->direction + "00";
                    imposter->texture = ResourceManager::GetTexture(imposter->textureString);
                    imposter->frames = 0;
                }
            }

            if (imposter->playerCollision(player)) {
                this->State = GAME_LOSE;
            }
        }

        if (!buttons) {
            for (auto &wall : walls) {
                if (wall->isWall and wall->position.x == (30 + 50 * (maze->columns - 2)) and
                    wall->position.y == (20 + 50 * (maze->rows - 1))) {
                    wall->isWall = false;
                    wall->texture = ResourceManager::GetTexture("bluetile");
                    break;
                }
            }

            if (maze->detectWin(player)) {
                this->State = GAME_WIN;
            }
        }
        int timeStamp = int(glfwGetTime());
        if (timeStamp > 100) {
            this->State = GAME_LOSE;
        }
        if (timeStamp > lastSecondPassed) {
            lastSecondPassed = timeStamp;
            player->score += (light ? 1 : 3);
        }
        lightToggle();
    }
}

void Game::Quit(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Game::CreateObstacles() {
    if (this->State == GAME_ACTIVE) {
        for (auto el: obsGrassPlots) {
            int percentageChance = rand() % 100;
            if (percentageChance < 20) {
                int option = rand() % 2;
                int x = el.first + 25 - 15, y = el.second + 25 - 15;
                if (option == 0) {
                    Obstacle *tempObs = new Obstacle("coin", glm::vec2(x, y), glm::vec2(30, 30), "coin", 0.0f,
                                                     glm::vec3(1.0f, 1.0f, 1.0f));
                    obstacles.push_back(tempObs);
                }
                if (option == 1) {
                    Obstacle *tempObs = new Obstacle("trap", glm::vec2(x, y), glm::vec2(30, 30), "trap", 0.0f,
                                                     glm::vec3(1.0f, 1.0f, 1.0f));
                    obstacles.push_back(tempObs);
                }
            }
        }
    }
}

void Game::lightToggle() {
    if (this->State == GAME_ACTIVE) {
        if (light == false) {
            player->color = glm::vec3(1.0f, 1.0f, 1.0f);
            int destR = -1, destC = -1;
            float px = player->position.x + player->size.x / 2;
            float py = player->position.y + player->size.y / 2;
            for (int i = 0; i < maze->rows * maze->columns; i++) {
                int ri = i / maze->columns, ci = i % maze->columns;
                if (maze->maze[ri][ci] != '#') {
                    float wallLeft = 30.0f + 50.0f * ci;
                    float wallRight = wallLeft + 50;
                    float wallTop = 20.0f + 50.0f * ri;
                    float wallBottom = wallTop + 50;
                    if (wallLeft < px and px < wallRight and wallTop < py and py < wallBottom) {
                        destR = ri;
                        destC = ci;
                        break;
                    }
                }
            }
            if (destR == -1 or destC == -1) {
                px = player->position.x;
                py = player->position.y;
                for (int i = 0; i < maze->rows * maze->columns; i++) {
                    int ri = i / maze->columns, ci = i % maze->columns;
                    if (maze->maze[ri][ci] != '#') {
                        float wallLeft = 30.0f + 50.0f * ci;
                        float wallRight = wallLeft + 50;
                        float wallTop = 20.0f + 50.0f * ri;
                        float wallBottom = wallTop + 50;
                        if (wallLeft < px and px < wallRight and wallTop < py and py < wallBottom) {
                            destR = ri;
                            destC = ci;
                            break;
                        }
                    }
                }
            }
            int posx = 30 + 50 * destC;
            int posy = 20 + 50 * destR;
            for (auto &wall : walls) {
                if (wall->position == glm::vec2(posx, posy) or wall->position == glm::vec2(posx + 50, posy) or
                    wall->position == glm::vec2(posx - 50, posy) or wall->position == glm::vec2(posx, posy + 50) or
                    wall->position == glm::vec2(posx, posy - 50) or wall->position == glm::vec2(posx + 50, posy + 50)
                    or wall->position == glm::vec2(posx + 50, posy - 50) or
                    wall->position == glm::vec2(posx - 50, posy + 50) or
                    wall->position == glm::vec2(posx - 50, posy - 50))
                    wall->color = glm::vec3(1.0f, 1.0f, 1.0f);
                else
                    wall->color = glm::vec3(0.5f, 0.5f, 0.5f);
            }
            float ipx = imposter->position.x + imposter->size.x / 2;
            float ipy = imposter->position.y + imposter->size.y / 2;
            int flag = 0;
            for (auto wall: walls) {
                if (wall->color == glm::vec3(1.0f, 1.0f, 1.0f)) {
                    float wallLeft = wall->position.x;
                    float wallRight = wallLeft + 50;
                    float wallTop = wall->position.y;
                    float wallBottom = wallTop + 50;
                    if (wallLeft < ipx and ipx < wallRight and wallTop < ipy and ipy < wallBottom) {
                        imposter->visible = true;
                        flag = 1;
                        break;
                    }
                }
            }
            if (!flag) imposter->visible = false;
            float ibpx = impButton->position.x + impButton->size.x / 2;
            float ibpy = impButton->position.y + impButton->size.y / 2;
            int ibflag = 0;
            for (auto wall: walls) {
                if (wall->color == glm::vec3(1.0f, 1.0f, 1.0f)) {
                    float wallLeft = wall->position.x;
                    float wallRight = wallLeft + 50;
                    float wallTop = wall->position.y;
                    float wallBottom = wallTop + 50;
                    if (wallLeft < ibpx and ibpx < wallRight and wallTop < ibpy and ibpy < wallBottom) {
                        impButton->visible = true;
                        ibflag = 1;
                        break;
                    }
                }
            }
            if (!ibflag) impButton->visible = false;

            float cbpx = coinButton->position.x + coinButton->size.x / 2;
            float cbpy = coinButton->position.y + coinButton->size.y / 2;
            int cbflag = 0;
            for (auto wall: walls) {
                if (wall->color == glm::vec3(1.0f, 1.0f, 1.0f)) {
                    float wallLeft = wall->position.x;
                    float wallRight = wallLeft + 50;
                    float wallTop = wall->position.y;
                    float wallBottom = wallTop + 50;
                    if (wallLeft < cbpx and cbpx < wallRight and wallTop < cbpy and cbpy < wallBottom) {
                        coinButton->visible = true;
                        cbflag = 1;
                        break;
                    }
                }
            }
            if (!cbflag) coinButton->visible = false;
            for (auto &obstacle : obstacles) {
                float opx = obstacle->position.x + obstacle->size.x / 2;
                float opy = obstacle->position.y + obstacle->size.y / 2;
                int oflag = 0;
                for (auto wall: walls) {
                    if (wall->color == glm::vec3(1.0f, 1.0f, 1.0f)) {
                        float wallLeft = wall->position.x;
                        float wallRight = wallLeft + 50;
                        float wallTop = wall->position.y;
                        float wallBottom = wallTop + 50;
                        if (wallLeft < opx and opx < wallRight and wallTop < opy and opy < wallBottom) {
                            obstacle->visible = true;
                            oflag = 1;
                            break;
                        }
                    }
                }
                if (!oflag) obstacle->visible = false;
            }

        } else {
            player->color = glm::vec3(1.0f, 1.0f, 1.0f);
            for (auto &wall : walls) {
                wall->color = glm::vec3(1.0f, 1.0f, 1.0f);
            }
            for (auto &obstacle : obstacles) {
                obstacle->visible = true;
            }
            impButton->visible = true;
            coinButton->visible = true;
            imposter->visible = true;
        }
    }
}

void Game::ProcessInput(GLFWwindow *window, float dt) {
    if (this->State == GAME_ACTIVE) {
        if (this->Keys[GLFW_KEY_Q]) {
            this->State = GAME_OVER;
        }

        if (this->Keys[GLFW_KEY_L]) {
            light = !light;
            this->Keys[GLFW_KEY_L] = false;
        }

        float velocity = 100 * dt;
        // move player around
        if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_S]) {
            if (player->wallCollisions(walls, velocity, this->Keys) == 0) {
                player->move(velocity, this->Width, this->Height, this->Keys);
            } else {
                player->textureString = "player" + player->direction + "00";
                player->texture = ResourceManager::GetTexture(player->textureString);
                player->frames = 0;
            }
            if (impButton->active and impButton->playerCollision(player)) {
                imposter->active = false;
                impButton->active = false;
                buttons--;
            }
            if (coinButton->active and coinButton->playerCollision(player)) {
                coinButton->active = false;
                CreateObstacles();
                buttons--;
            }
            for (auto &obstacle : obstacles) {
                if (obstacle->active) {
                    if (obstacle->playerCollision(player)) {
                        player->score += (obstacle->type == "trap" ? -10 : 20);
                        player->score = fmax(0, player->score);
                        obstacle->active = false;
                    }
                }
            }
        } else {
            player->textureString = "player" + player->direction + "00";
            player->texture = ResourceManager::GetTexture(player->textureString);
            player->frames = 0;
        }
    }
}

void Game::Render(GLFWwindow *window) {
    if (this->State == GAME_ACTIVE) {
        for (auto &wall : walls) {
            renderer->DrawSprite(wall);
        }
        for (auto &obstacle : obstacles) {
            if (obstacle->active and obstacle->visible)
                renderer->DrawSprite(obstacle);
        }
        if (coinButton->active and coinButton->visible)
            renderer->DrawSprite(coinButton);
        if (impButton->active and impButton->visible)
            renderer->DrawSprite(impButton);
        renderer->DrawSprite(player);
        if (imposter->active and imposter->visible)
            renderer->DrawSprite(imposter);
        std::stringstream scoreS;
        std::stringstream timeS;
        std::stringstream taskS;
        std::stringstream lightS;
        scoreS << player->score;
        timeS << int(glfwGetTime());
        taskS << 2 - buttons;
        lightS << (light ? "On" : "Off");
        Text->RenderText("Score:  " + scoreS.str() + "  Time:  " + timeS.str() + "  Tasks:  " + taskS.str() + " / 2" +
                         +"  Light:  " + lightS.str(),
                         this->Width - 425.0f, 25.0f, 0.8f,
                         glm::vec3(1.0f, 1.0f, 1.0f));
    } else if (this->State == GAME_LOSE) {
        renderer->DrawSprite(LoseScreen);
    } else if (this->State == GAME_WIN) {
        renderer->DrawSprite(WinScreen);
        std::stringstream scoreS;
        scoreS << (player->score + (100 - lastSecondPassed) * 10);
        Text->RenderText("Score:  " + scoreS.str(),
                         this->Width - 150.0f, 25.0f, 1.0f,
                         glm::vec3(1.0f, 1.0f, 1.0f));
    } else if (this->State == GAME_OVER) {
        renderer->DrawSprite(OverScreen);
    }
}
