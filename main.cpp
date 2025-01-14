#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <array>
#include <tuple>

struct Node {
    int x, y, weight;

    Node() = default;
    Node(int x, int y, int weight) : x(x), y(y), weight(weight) {}
};

struct Compare {
    bool operator()(const Node& a, const Node& b) const {
        return a.weight > b.weight;
    }
};

constexpr std::array<int, 4> dx = { 0, 1, 0, -1 };
constexpr std::array<int, 4> dy = { -1, 0, 1, 0 };

using Path = std::vector<std::pair<int, int>>;

Path restorePath(const std::vector<std::vector<std::pair<int, int>>>& prev, int startX, int startY, int endX, int endY) {
    Path path;
    for (int x = endX, y = endY; x != startX || y != startY; ) {
        path.emplace_back(y, x);
        std::tie(y, x) = prev[y][x];
    }
    path.emplace_back(startY, startX);
    std::reverse(path.begin(), path.end());
    return path;
}

Path dijkstra(const Node& start, const std::vector<std::vector<int>>& grid, int targetX, int targetY) {
    int rows = grid.size(), cols = grid[0].size();
    std::vector<std::vector<int>> distances(rows, std::vector<int>(cols, INT_MAX));
    std::vector<std::vector<std::pair<int, int>>> predecessors(rows, std::vector<std::pair<int, int>>(cols, { -1, -1 }));
    std::priority_queue<Node, std::vector<Node>, Compare> pq;

    distances[start.y][start.x] = 0;
    pq.push(start);

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.weight > distances[current.y][current.x]) continue;

        for (int k = 0; k < 4; ++k) {
            int newX = current.x + dx[k];
            int newY = current.y + dy[k];

            if (newX < 0 || newY < 0 || newX >= cols || newY >= rows || grid[newY][newX] == 0) continue;

            int newDist = distances[current.y][current.x] + grid[newY][newX];
            if (newDist < distances[newY][newX]) {
                distances[newY][newX] = newDist;
                predecessors[newY][newX] = { current.y, current.x };
                pq.push(Node(newX, newY, newDist));
            }
        }
    }

    if (distances[targetY][targetX] == INT_MAX) return {};
    return restorePath(predecessors, start.x, start.y, targetX, targetY);
}

bool isValidInput(int n, int m, int x1, int y1, int x2, int y2) {
    return x1 >= 0 && x1 < m && y1 >= 0 && y1 < n && x2 >= 0 && x2 < m && y2 >= 0 && y2 < n;
}

void printPath(const Path& path) {
    for (const auto& [y, x] : path) {
        std::cout << y << " " << x << "\n";
    }
    std::cout << ".";
}

int main() {
    int rows, cols;
    std::cin >> rows >> cols;

    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols));
    for (auto& row : grid) {
        for (auto& cell : row) {
            std::cin >> cell;
        }
    }

    int startY, startX, targetY, targetX;
    std::cin >> startY >> startX >> targetY >> targetX;

    if (!isValidInput(rows, cols, startX, startY, targetX, targetY)) {
        std::cout << "Incorrect input";
        return 0;
    }

    Path path = dijkstra(Node(startX, startY, 0), grid, targetX, targetY);
    if (path.empty()) {
        std::cout << "No path found";
        return 0;
    }

    printPath(path);
    return 0;
}