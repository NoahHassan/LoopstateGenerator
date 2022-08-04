#include <vector>
#include <cassert>
#include <unordered_map>
#include <random>
#include <fstream>
#include <iostream>

class Cell
{
public:
    Cell() = default;
    Cell(int t, int b, int l, int r)
    {
        map['t'] = t;
        map['b'] = b;
        map['l'] = l;
        map['r'] = r;
    }
public:
    void ToggleCell()
    {
        for(auto& link : map)
        {
            link.second = (link.second + 1) % 2;
        }
    }
    void ToggleLink(char c)
    {
        assert(map.count(c));
        int link = map[c];
        map[c] = (link + 1) % 2;
    }
    int GetLink(char c) const
    {
        assert(map.count(c) > 0);
        return map.at(c);
    }
private:
    std::unordered_map<char, int> map = 
    {
        std::make_pair<char, int>('t', 0),
        std::make_pair<char, int>('b', 0),
        std::make_pair<char, int>('l', 0),
        std::make_pair<char, int>('r', 0)
    };
};

class Field
{
public:
    Field(int width, int height)
        :
        width(width),
        height(height)
    {
        assert(width > 2);
        assert(height > 2);
        cells = std::vector<Cell>(width * height);
    }
public:
    void ToggleCell(int x, int y)
    {
        // to not deal with boundary conditions
        // shrink cell field
        assert(1 <= x);
        assert(1 <= y);
        assert(x < width - 1);
        assert(y < height - 1);

        // toggle cell at (x,y)
        cells[y * width + x].ToggleCell();

        // toggle adjacent cells (y = 0 is top left corner)
        cells[y * width + (x - 1)].ToggleLink('r');
        cells[y * width + (x + 1)].ToggleLink('l');
        cells[(y + 1) * width + x].ToggleLink('t');
        cells[(y - 1) * width + x].ToggleLink('b');
    }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    Cell GetCell(int x, int y) const
    {
        assert(0 <= x);
        assert(0 <= y);
        assert(x < width);
        assert(y < height);
        return cells[y * width + x];
    }
    // returns vector of indices for links to draw
    std::vector<int> GetLinks() const
    {
        std::vector<int> indices;
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                Cell currentCell = GetCell(x,y);
                if(currentCell.GetLink('t') > 0)
                {
                    // top left corner of cell
                    // (use width + 1, because now in grid-coords)
                    indices.emplace_back(y * (width + 1) + x);
                    // top right corner of cell
                    indices.emplace_back(y * (width + 1) + x + 1);
                }
                if(currentCell.GetLink('l') > 0)
                {
                    // top left corner of cell
                    indices.emplace_back(y * (width + 1) + x);
                    // bottom left corner of cell
                    indices.emplace_back((y + 1) * (width + 1) + x);
                }
            }
        }

        return std::move(indices);
    }
private:
    int width;
    int height;
    std::vector<Cell> cells;
};

int main()
{
    int size;
    int nCells;

    std::cout << "size: ";
    std::cin >> size;
    std::cout << "nCells: ";
    std::cin >> nCells;

    Field cellField(size, size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dst(1, size - 2); // ignore the boundary

    for(int n = 0; n < nCells; n++)
    {
        int x = dst(gen);
        int y = dst(gen);

        cellField.ToggleCell(x,y);
    }

    std::vector<int> indices = cellField.GetLinks();
    std::ofstream outFile("loop.txt", std::ios::binary);
    outFile << (size + 1); // size + 1, since this will be the size of grid points (not cells)
    for(const int i : indices)
    {
        outFile << '\n' << i;
    }

    outFile.close();
    return 0;
}