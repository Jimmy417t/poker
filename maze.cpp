#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;


enum Color
{
    black = 90, red, green, yellow, blue, purple, darkblue, white
};

// 顏色定義 ANSI
void setColor(Color color) 
{
    cout << "\033[" << color << "m";
}

class Maze {
private:
    int width, height;
    vector<vector<char>> map;
    vector<vector<char>> originalMap;
    pair<int, int> start, goal;
    
    const int dx[4] = {0, 0, -1, 1};// 上 下 左 右 
    const int dy[4] = {-1, 1, 0, 0};// 上 下 左 右  
    
    bool isValid(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    
    bool isInBounds(int x, int y) {
        return x > 0 && x < width-1 && y > 0 && y < height-1;
    }
    
    void resetMap() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                map[y][x] = originalMap[y][x];
            }
        }
    }
    
    // 隨機打亂方向順序，增加隨機性
    vector<int> shuffleDirections() {
        vector<int> dirs = {0, 1, 2, 3};
        random_shuffle(dirs.begin(), dirs.end());
        return dirs;
    }
    
public:
    Maze(int w, int h) : width(w), height(h) {
        if (width % 2 == 0) width++;
        if (height % 2 == 0) height++;
        
        map.resize(height, vector<char>(width, '#'));
        originalMap.resize(height, vector<char>(width, '#'));
        start = make_pair(1, 1);
        goal = make_pair(width - 2, height - 2);
    }
    
    // 進階生成：產生大量分岔和死路
    void generateComplex() {
        // 先全部初始化為牆
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                map[y][x] = '#';
            }
        }
        
        stack<pair<int, int>> st;
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        
        // 從起點開始
        st.push(start);
        visited[start.second][start.first] = true;
        map[start.second][start.first] = ' ';
        
        while (!st.empty()) {
            pair<int, int> current = st.top();
            int x = current.first;
            int y = current.second;
            
            // 找出所有未訪問的鄰居（距離2格）
            vector<pair<int, pair<int, int>>> neighbors;
            vector<int> dirs = shuffleDirections();
            for (int i : dirs) {
                int nx = x + dx[i] * 2;
                int ny = y + dy[i] * 2;
                if (isInBounds(nx, ny) && !visited[ny][nx] && map[ny][nx] == '#') {
                    neighbors.push_back(make_pair(i, make_pair(nx, ny)));
                }
            }
            
            if (!neighbors.empty()) {
                // 隨機選擇一個鄰居
                int idx = rand() % neighbors.size();
                int dir = neighbors[idx].first;
                int nx = neighbors[idx].second.first;
                int ny = neighbors[idx].second.second;
                int wx = x + dx[dir];
                int wy = y + dy[dir];
                
                // 打通牆壁和目標格
                map[wy][wx] = ' ';
                map[ny][nx] = ' ';
                visited[ny][nx] = true;
                st.push(make_pair(nx, ny));
                
                // 增加分岔 有機率也打通其他方向的牆
                if (neighbors.size() > 1 && rand() % 100 < 25) 
                {
                    int idx2 = rand() % neighbors.size();
                    if (idx2 != idx) {
                        int dir2 = neighbors[idx2].first;
                        int nx2 = neighbors[idx2].second.first;
                        int ny2 = neighbors[idx2].second.second;
                        int wx2 = x + dx[dir2];
                        int wy2 = y + dy[dir2];
                        map[wy2][wx2] = ' ';
                        map[ny2][nx2] = ' ';
                        visited[ny2][nx2] = true;
                        st.push(make_pair(nx2, ny2));
                    }
                }
            } else {
                st.pop();
            }
        }
        
        // 第二階段：隨機打通一些牆壁，創造更多迴路
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                if (map[y][x] == '#') {
                    // 檢查上下或左右是否都是路
                    int pathCount = 0;
                    if (map[y-1][x] == ' ') pathCount++;
                    if (map[y+1][x] == ' ') pathCount++;
                    if (map[y][x-1] == ' ') pathCount++;
                    if (map[y][x+1] == ' ') pathCount++;
                    
                    // 如果兩邊都有路，有機率打通（創造捷徑）
                    if (pathCount >= 2 && rand() % 100 < 8) {
                        map[y][x] = ' ';
                    }
                }
            }
        }
        
        map[start.second][start.first] = 'S';
        map[goal.second][goal.first] = 'G';
        
        // 保存原始地圖
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                originalMap[y][x] = map[y][x];
            }
        }
    }
    
    // 計算分岔數量（複雜度指標）
    int getBranchCount() {
        int branches = 0;
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                if (map[y][x] == ' ') {
                    int exits = 0;
                    for (int i = 0; i < 4; i++) {
                        int nx = x + dx[i];
                        int ny = y + dy[i];
                        if (map[ny][nx] == ' ' || map[ny][nx] == 'S' || map[ny][nx] == 'G') {
                            exits++;
                        }
                    }
                    if (exits >= 3) branches++;
                }
            }
        }
        return branches;
    }
    
    bool dfsSolve() {
        resetMap();
        stack<pair<int, int>> st;
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, make_pair(-1, -1)));
        
        st.push(start);
        visited[start.second][start.first] = true;
        
        while (!st.empty()) {
            pair<int, int> current = st.top();
            st.pop();
            int x = current.first;
            int y = current.second;
            
            if (x == goal.first && y == goal.second) {
                pair<int, int> cur = goal;
                int steps = 0;
                while (cur != start) {
                    pair<int, int> p = parent[cur.second][cur.first];
                    int px = p.first;
                    int py = p.second;
                    if (map[py][px] != 'S' && map[py][px] != 'G') {
                        map[py][px] = 'D';
                    }
                    cur = make_pair(px, py);
                    steps++;
                }
                cout << "DFS 路徑長度: " << steps << " 步" << endl;
                return true;
            }
            
            // 隨機打亂方向，增加不確定性
            vector<int> dirs = shuffleDirections();
            for (int i : dirs) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (isValid(nx, ny) && !visited[ny][nx] && (map[ny][nx] == ' ' || map[ny][nx] == 'G')) {
                    visited[ny][nx] = true;
                    parent[ny][nx] = make_pair(x, y);
                    st.push(make_pair(nx, ny));
                }
            }
        }
        return false;
    }
    
    bool bfsSolve() {
        resetMap();
        queue<pair<int, int>> q;
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, make_pair(-1, -1)));
        
        q.push(start);
        visited[start.second][start.first] = true;
        
        while (!q.empty()) {
            pair<int, int> current = q.front();
            q.pop();
            int x = current.first;
            int y = current.second;
            
            if (x == goal.first && y == goal.second) {
                pair<int, int> cur = goal;
                int steps = 0;
                while (cur != start) {
                    pair<int, int> p = parent[cur.second][cur.first];
                    int px = p.first;
                    int py = p.second;
                    if (map[py][px] != 'S' && map[py][px] != 'G') {
                        map[py][px] = 'B';
                    }
                    cur = make_pair(px, py);
                    steps++;
                }
                cout << "BFS 路徑長度: " << steps << " 步" << endl;
                return true;
            }
            
            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (isValid(nx, ny) && !visited[ny][nx] && (map[ny][nx] == ' ' || map[ny][nx] == 'G')) {
                    visited[ny][nx] = true;
                    parent[ny][nx] = make_pair(x, y);
                    q.push(make_pair(nx, ny));
                }
            }
        }
        return false;
    }
    
    void display() {
        cout << endl;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                char c = map[y][x];
                if (c == '#') {
                    setColor(black);
                    cout << "█";
                } else if (c == 'D') {
                    setColor(yellow);
                    cout << "D";
                } else if (c == 'B') {
                    setColor(green);
                    cout << "B";
                } else if (c == 'S') {
                    setColor(red);
                    cout << "S";
                } else if (c == 'G') {
                    setColor(red);
                    cout << "G";
                } else {
                    setColor(white);
                    cout << " ";
                }
            }
            setColor(white);
            cout << endl;
        }
        setColor(white);
        cout << "\n圖例: █=牆  空格=路  S=起點  G=終點  ";
        setColor(yellow);
        cout << "D=DFS路徑";
        setColor(white);
        cout << "  ";
        setColor(green);
        cout << "B=BFS最短路徑";
        setColor(white);
        cout << endl;
    }
    
    void showStats() {
        cout << "\n=== 迷宮資訊 ===" << endl;
        cout << "尺寸: " << width << " x " << height << endl;
        cout << "起點: (1, 1)" << endl;
        cout << "終點: (" << width-2 << ", " << height-2 << ")" << endl;
        cout << "分岔點數量: " << getBranchCount() << endl;
    }
};

int main() {
    srand(time(nullptr));
    
    int size = 0;
    
    cout << "=== 迷宮生成器 ===" << endl;
    cout << "請輸入迷宮大小 (奇數，不超過151): ";
    cin >> size;
    
    if (size < 51) size = 51;
    if (size % 2 == 0) size++;
    if (size > 151) size = 151;
    
    Maze maze(size, size);
    
    cout << "\n=== 迷宮生成中  ===" << endl;
    maze.generateComplex();
    maze.showStats();
    
    cout << "\n按 Enter 顯示迷宮...";
    cin.ignore();
    cin.get();
    
    maze.display();
    
    cout << "\n按 Enter 開始 DFS 求解...";
    cin.get();
    
    cout << "\n========== DFS 求解 ==========" << endl;
    auto start = high_resolution_clock::now();
    if (maze.dfsSolve()) {
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        maze.display();
        cout << "\nDFS 執行時間: " << duration.count() << " 毫秒" << endl;
    } else {
        cout << "DFS 找不到路徑！" << endl;
    }
    
    cout << "\n按 Enter 開始 BFS 求解...";
    cin.get();
    
    cout << "\n========== BFS 求解 ==========" << endl;
    start = high_resolution_clock::now();
    if (maze.bfsSolve()) {
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        maze.display();
        cout << "\nBFS 執行時間: " << duration.count() << " 毫秒" << endl;
    } else {
        cout << "BFS 找不到路徑！" << endl;
    }
    
    return 0;
}