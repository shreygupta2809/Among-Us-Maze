#include "maze.h"
#include <queue>

using namespace std;

Maze::Maze(int R, int C) {
    this->rows = R;
    this->columns = C;

    int r = (R - 1) / 2, c = (C - 1) / 2;

//    char **maze = new char *[R];
//
//    for (int i = 0; i < R; i++) {
//        maze[i] = new char[C];
//    }
    vector<vector<char>> maze(R, vector<char>(C, ' '));

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (!(i & 1) || !(j & 1))
                maze[i][j] = '#';
            else
                maze[i][j] = ' ';
        }
    }

    for (int i = 1; i < R; i += 2) {
        for (int j = 1; j < C; j += 2) {
            maze[i][j] = ' ';
        }
    }

    vector<pair<int, pair<int, int>>> cell_list;
    vector<bool> visited(r * c, false);
    stack<pair<int, pair<int, int>>> m_stack;
    random_device rdev;
    mt19937 rng(rdev());
    uniform_int_distribution<mt19937::result_type> dist100(1, 100);

    int nVisited = 0;
    int k = 0;

    for (int i = 1; i < R; i += 2) {
        for (int j = 1; j < C; j += 2) {
            cell_list.push_back(make_pair(k, make_pair(i, j)));
            k++;
        }
    }

    int randIdx = dist100(rng) % r * c;
    m_stack.push(cell_list[randIdx]);
    visited[randIdx] = true;
    nVisited++;

    // Algo
    while (nVisited < r * c) {

        vector<int> neighbours;
        // North
        if (m_stack.top().second.first > 1) {
            if (maze[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(0);
            }
        }
        // East
        if (m_stack.top().second.second < C - 2) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]) {
                neighbours.push_back(1);
            }
        }
        // South
        if (m_stack.top().second.first < R - 2) {
            if (maze[m_stack.top().second.first + 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(2);
            }
        }
        // West
        if (m_stack.top().second.second > 1) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second - 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]) {
                neighbours.push_back(3);
            }
        }
        // Neighbours available?
        if (!neighbours.empty()) {
            // Choose a random direction
            int next_cell_dir = neighbours[dist100(rng) % neighbours.size()];
            // Create a path between this cell and neighbour
            switch (next_cell_dir) {
                case 0: // North
                    maze[m_stack.top().second.first - 1][m_stack.top().second.second + 0] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0,
                                                  cell_list)]);
                    break;
                case 1: // East
                    maze[m_stack.top().second.first + 0][m_stack.top().second.second + 1] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2,
                                                  cell_list)]);
                    break;
                case 2: // South
                    maze[m_stack.top().second.first + 1][m_stack.top().second.second + 0] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0,
                                                  cell_list)]);
                    break;
                case 3: // West
                    maze[m_stack.top().second.first + 0][m_stack.top().second.second - 1] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2,
                                                  cell_list)]);
                    break;
                default:
                    break;
            }

            visited[m_stack.top().first] = true;
            nVisited++;
        } else {
            m_stack.pop();
        }
    }
    maze[0][1] = 'S';
    maze[R - 1][C - 2] = 'E';

    for (int r = 1; r < R - 1; r++) {
        for (int c = 1; c < C - 1; c++) {
            if (maze[r][c] == '#') {
                if ((maze[r - 1][c] == '#' and maze[r][c + 1] == '#') or
                    (maze[r][c + 1] == '#' and maze[r + 1][c] == '#') or
                    (maze[r + 1][c] == '#' and maze[r][c - 1] == '#') or
                    (maze[r][c - 1] == '#' and maze[r - 1][c] == '#'))
                    continue;
                int removeChance = rand() % 100;
                if (removeChance < 10)
                    maze[r][c] = ' ';
            }
        }
    }
    this->maze = maze;

    vector<vector<pair<int, int>>> adjacency(R * C + 1);
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (maze[r][c] != '#') {
                for (int rr = 0; rr < R; rr++) {
                    for (int cc = 0; cc < C; cc++) {
                        if (maze[rr][cc] != '#') {
                            if ((rr == r and cc == c - 1) or (rr == r and cc == c + 1) or (rr == r - 1 and cc == c) or
                                (rr == r + 1 and cc == c)) {
                                adjacency[r * C + c].push_back(make_pair(rr, cc));
                            }
                        }
                    }
                }
            }
        }
    }
    this->adjList = adjacency;
//    displayMaze();
}

Maze::~Maze() {

}

// A utility function to get the index of cell with indices x, y;
int Maze::getIdx(int x, int y, const vector<pair<int, pair<int, int>>> &cell_list) {
    for (auto &i : cell_list) {
        if (i.second.first == x && i.second.second == y)
            return i.first;
    }
    return -1;
}

void Maze::displayMaze() const {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            cout << this->maze[i][j] << " ";
        }
        cout << "\n";
    }
}

std::string Maze::computePath(Player *player, Imposter *imposter) {
    std::queue<std::pair<int, int>> q;
    int R = this->rows, C = this->columns;
    bool visited[R * C + 1];
    int distance[R * C + 1];
    int parent[R * C + 1];

    for (int i = 0; i < R * C + 1; i++) {
        visited[i] = false;
        distance[i] = INT_MAX;
        parent[i] = -1;
    }

    glm::vec2 startPos = imposter->position;
    int x = startPos.x + imposter->size.x / 2 - 25 - 30;
    int y = startPos.y + imposter->size.y / 2 - 25 - 20;
    int c = x / 50, r = y / 50;
    int src = r * C + c;

    glm::vec2 endPos = player->position;
    float px = endPos.x + player->size.x / 2;
    float py = endPos.y + player->size.y / 2;

    int dest = -1, destR = -1, destC = -1;

    for (int i = 0; i < R * C; i++) {
        int ri = i / C, ci = i % C;
        if (maze[ri][ci] != '#') {
            float wallLeft = 30.0f + 50.0f * ci;
            float wallRight = wallLeft + 50;
            float wallTop = 20.0f + 50.0f * ri;
            float wallBottom = wallTop + 50;
            if (wallLeft < px and px < wallRight and wallTop < py and py < wallBottom) {
                dest = i;
                destR = ri;
                destC = ci;
                break;
            }
        }
    }

    if (dest == -1) {
        px = player->position.x;
        py = player->position.y;
        for (int i = 0; i < R * C; i++) {
            int ri = i / C, ci = i % C;
            if (maze[ri][ci] != '#') {
                float wallLeft = 30.0f + 50.0f * ci;
                float wallRight = wallLeft + 50;
                float wallTop = 20.0f + 50.0f * ri;
                float wallBottom = wallTop + 50;
                if (wallLeft < px and px < wallRight and wallTop < py and py < wallBottom) {
                    dest = i;
                    destR = ri;
                    destC = ci;
                    break;
                }
            }
        }
        if (dest == -1)
            return "continue";
    }

    visited[src] = true;
    distance[src] = 0;
    q.push(make_pair(r, c));

    int flag = 0;

    while (!q.empty() and !flag) {
        pair<int, int> front = q.front();
        q.pop();
        int index = front.first * C + front.second;
        for (auto neighbour: adjList[index]) {
            int nIndex = neighbour.first * C + neighbour.second;
            if (!visited[nIndex]) {
                visited[nIndex] = true;
                distance[nIndex] = distance[index] + 1;
                parent[nIndex] = index;
                q.push(neighbour);

                if (nIndex == dest) {
                    flag = 1;
                    break;
                }
            }
        }
    }

    vector<int> path;
    int tempDest = dest;
    path.push_back(tempDest);
    while (parent[tempDest] != -1) {
        path.push_back(parent[tempDest]);
        tempDest = parent[tempDest];
    }

    int targetPoint = path[path.size() - 2];
    int targetR = targetPoint / C, targetC = targetPoint % C;

    imposter->target = glm::vec2(30 + targetC * 50 + 25, targetR * 50 + 20 + 25);

    if (targetR == r and targetC == c - 1)
        return "left";
    else if (targetR == r and targetC == c + 1)
        return "right";
    else if (targetR == r - 1 and targetC == c)
        return "up";
    else if (targetR == r + 1 and targetC == c)
        return "down";
    else
        return "continue";
}

bool Maze::detectWin(Player *pPlayer) {
    int px = pPlayer->position.x + pPlayer->size.x / 2;
    int py = pPlayer->position.y + pPlayer->size.y / 2;

    for (int i = 0; i < this->rows * this->columns; i++) {
        int ri = i / this->columns, ci = i % this->columns;
        if (maze[ri][ci] == 'E') {
            int wallLeft = 30 + 50 * ci;
            int wallRight = wallLeft + 50;
            int wallTop = 20 + 50 * ri;
            int wallBottom = wallTop + 50;
            if (wallLeft < px and px < wallRight and wallTop < py and py < wallBottom) {
                return true;
                break;
            }
        }
    }
    return false;
}
