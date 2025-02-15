#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>

const char STONE = ' ';
const char FLOOR = '.';
const char WALL = '#';

struct Room {
    int x, y, w, h;
};

class Generator {
public:
    int width, height, maxRooms, minRoomSize, maxRoomSize, roomSpacing, corridorSpacing, randomConnections, randomSpurs;
    std::vector<std::vector<char>> level;
    std::vector<Room> rooms;
    
    Generator(int w = 64, int h = 64, int maxR = 15, int minR = 5, int maxRSize = 10, int spacing = 3, int cSpacing = 2, int rC = 1, int rS = 3)
        : width(w), height(h), maxRooms(maxR), minRoomSize(minR), maxRoomSize(maxRSize), roomSpacing(spacing), corridorSpacing(cSpacing),
          randomConnections(rC), randomSpurs(rS) {
        level.resize(height, std::vector<char>(width, STONE));
    }

    Room genRoom() {
        int w = rand() % (maxRoomSize - minRoomSize + 1) + minRoomSize;
        int h = rand() % (maxRoomSize - minRoomSize + 1) + minRoomSize;
        int x = rand() % (width - w - roomSpacing - 1) + 1;
        int y = rand() % (height - h - roomSpacing - 1) + 1;
        return {x, y, w, h};
    }

    bool roomOverlaps(const Room& room) {
        for (const auto& r : rooms) {
            if (room.x < (r.x + r.w + roomSpacing) && r.x < (room.x + room.w + roomSpacing) && 
                room.y < (r.y + r.h + roomSpacing) && r.y < (room.y + room.h + roomSpacing)) {
                return true;
            }
        }
        return false;
    }

    void createRooms() {
        for (int i = 0; i < maxRooms * 5; ++i) {
            Room room = genRoom();
            if (!roomOverlaps(room)) {
                rooms.push_back(room);
            }
            if (rooms.size() >= maxRooms) break;
        }
    }

    void connectRooms() {
        for (size_t i = 0; i < rooms.size() - 1; ++i) {
            int x1 = rooms[i].x + rooms[i].w / 2;
            int y1 = rooms[i].y + rooms[i].h / 2;
            int x2 = rooms[i + 1].x + rooms[i + 1].w / 2;
            int y2 = rooms[i + 1].y + rooms[i + 1].h / 2;

            if (rand() % 2) {
                while (x1 != x2) {
                    if (level[y1][x1] == STONE) level[y1][x1] = FLOOR;
                    x1 += (x2 > x1) ? 1 : -1;
                }
                while (y1 != y2) {
                    if (level[y1][x1] == STONE) level[y1][x1] = FLOOR;
                    y1 += (y2 > y1) ? 1 : -1;
                }
            } else {
                while (y1 != y2) {
                    if (level[y1][x1] == STONE) level[y1][x1] = FLOOR;
                    y1 += (y2 > y1) ? 1 : -1;
                }
                while (x1 != x2) {
                    if (level[y1][x1] == STONE) level[y1][x1] = FLOOR;
                    x1 += (x2 > x1) ? 1 : -1;
                }
            }
        }
    }

    void placeRooms() {
        for (const auto& room : rooms) {
            for (int i = 0; i < room.w; ++i) {
                for (int j = 0; j < room.h; ++j) {
                    level[room.y + j][room.x + i] = FLOOR;
                }
            }
        }
    }

    void placeWalls() {
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                if (level[y][x] == FLOOR) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (level[y + dy][x + dx] == STONE) {
                                level[y + dy][x + dx] = WALL;
                            }
                        }
                    }
                }
            }
        }
    }

    void generate() {
        createRooms();
        placeRooms();
        connectRooms();
        placeWalls();
    }

    void printDungeon() {
        for (const auto& row : level) {
            for (char tile : row) {
                std::cout << tile;
            }
            std::cout << std::endl;
        }
    }
};