#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdint>

using llu = uint32_t;

struct TrieNode {
    std::unordered_map<llu, llu> Children;
    llu PatternNumber;
    bool IsEndOfWord;
    llu SuffixLink;
    std::unordered_map<llu, llu> AutoMove;
    llu Parent;
    llu ParentValue;
    llu GoodSuffixLink;

    TrieNode() {
        IsEndOfWord = false;
        SuffixLink = -1;
        GoodSuffixLink = -1;
        Parent = 0;
        ParentValue = -1;
        PatternNumber = -1;
    }

    TrieNode(llu parent, llu parentValue) {
        IsEndOfWord = false;
        SuffixLink = -1;
        Parent = parent;
        ParentValue = parentValue;
        GoodSuffixLink = -1;
        PatternNumber = -1;
    }

    ~TrieNode() = default;
};

struct Triplet {
    llu Value;
    llu LineNumber;
    llu NumberInLine;

    Triplet() = default;
    Triplet(llu value, llu lineNumber, llu numberInLine) {
        Value = value;
        LineNumber = lineNumber;
        NumberInLine = numberInLine;
    }

    ~Triplet() = default;
};

std::vector<struct TrieNode> Trie;
std::vector<std::vector<llu>> Patterns;

void Init() {
    Trie.push_back(TrieNode());
}

void AddPatternToTrie (std::vector<llu>& vec) {
    llu number = 0;

    for (llu i = 0; i < vec.size(); ++i) {
        if (!Trie[number].Children.count(vec[i])) {
            Trie.push_back(TrieNode(number, vec[i]));
            Trie[number].Children[vec[i]] = Trie.size() - 1;
        }

        number = Trie[number].Children[vec[i]];
    }

    Trie[number].IsEndOfWord = true;
    Patterns.push_back(vec);
    Trie[number].PatternNumber = Patterns.size() - 1;
}

bool CheckPatternInTrie (std::vector<llu>& vec) {
    llu number = 0;

    for (llu i = 0; i < vec.size(); ++i) {
        if (!Trie[number].Children.count(vec[i])) {
            return false;
        }

        number = Trie[number].Children[vec[i]];
    }

    return true;
}

llu GetAutoMove(llu v, llu value);

llu GetSuffixLink(llu v) {
    if (Trie[v].SuffixLink == -1) {
        if (v == 0 || Trie[v].Parent == 0) {

            Trie[v].SuffixLink = 0;

        } else {

            Trie[v].SuffixLink = GetAutoMove(GetSuffixLink(Trie[v].Parent), Trie[v].ParentValue);

        }
    }

    return Trie[v].SuffixLink;
}

llu GetAutoMove(llu v, llu value) {
    if (!Trie[v].AutoMove.count(value)) {
        if (Trie[v].Children.count(value)) {

            Trie[v].AutoMove[value] = Trie[v].Children[value];

        } else {

            if (v == 0) {

                Trie[v].AutoMove[value] = 0;

            } else {

                Trie[v].AutoMove[value] = GetAutoMove(GetSuffixLink(v), value);

            }

        }
    }

    return Trie[v].AutoMove[value];
}

llu GetGoodSuffixLink(llu v) {
    if (Trie[v].GoodSuffixLink == -1) {
        llu u = GetSuffixLink(v);

        if (u == 0) {
            Trie[v].GoodSuffixLink = 0;
        } else {
            Trie[v].GoodSuffixLink = (Trie[u].IsEndOfWord) ? u : GetGoodSuffixLink(u);
        }
    }

    return Trie[v].GoodSuffixLink;
}

void Check(llu v, llu i, llu currentStrNumber, llu currentPatternNumber) {
    for (llu u = v; u != 0; u = GetGoodSuffixLink(u)) {
        if (Trie[u].IsEndOfWord) {
            std::cout << currentStrNumber << ", " << i;

            if (currentPatternNumber != -1) {
                std::cout << ' ' << currentPatternNumber;
            }

            std::cout << std::endl;
        }
    }
}

void FindAllPos(std::vector<Triplet>& vec, bool manyPatternsFlag) {
    llu u = 0;
    /*
    std::vector<std::pair<llu, llu>> helper;

    for (llu i = 0; i < vec.size(); ++i) {
        for (llu j = 0; j < vec[i].size(); ++j) {
            helper.push_back({vec[i][j], i + 1});
        }
    }

    for (llu i = 0; i < vec.size(); ++i) {
        for (llu j = 0; j < vec[i].size(); ++j) {
            ++elemNumber;

            if (elemNumber - Patterns[0].size() >= 0) {
                currentStrNumber = helper[elemNumber - Patterns[0].size()].second;
            } else {
                currentStrNumber = helper[elemNumber].second;
            }

            u = GetAutoMove(u, vec[i][j]);
            Check(u, elemNumber, currentStrNumber);
        }
    }*/

    llu currentLineNumber, elemNumber, currentPatternNumber;

    for (llu i = 0; i < vec.size(); ++i) {
        u = GetAutoMove(u, vec[i].Value);

        if (Trie[u].PatternNumber != -1 && i - Patterns[Trie[u].PatternNumber].size() + 1 >= 0) {
            currentLineNumber = vec[i - Patterns[Trie[u].PatternNumber].size() + 1].LineNumber;
        } else {
            currentLineNumber = vec[i].LineNumber;
        }

        if (Trie[u].PatternNumber != -1 && i - Patterns[Trie[u].PatternNumber].size() + 1 >= 0) {
            elemNumber = vec[i - Patterns[Trie[u].PatternNumber].size() + 1].NumberInLine;
        } else {
            elemNumber = vec[i].NumberInLine;
        }

        if (manyPatternsFlag) {
            currentPatternNumber = Trie[u].PatternNumber + 1;
        } else {
            currentPatternNumber = -1;
        }

        Check(u, elemNumber, currentLineNumber, currentPatternNumber);
    }

}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    Init();

    std::vector<std::vector<llu>> allInput;
    std::vector<Triplet> text;
    std::string str;
    llu lastPatternIndex = -1, currentLineIndex = -1;
    bool endlineFlag = true;

    while (std::getline(std::cin, str, '\n')) {
        ++currentLineIndex;

        if (endlineFlag && !str.size() && allInput.size()) {
            lastPatternIndex = currentLineIndex - 1;
            endlineFlag = false;
        }

        std::istringstream input(str);
        std::vector<llu> currentLine;
        std::string helper;

        while (input >> helper) {
            currentLine.push_back(std::stoul(helper));
        }

        allInput.push_back(currentLine);
    }

    if (lastPatternIndex == -1 && allInput.size()) {
        AddPatternToTrie(allInput[0]);

        for (llu i = 1; i < allInput.size(); ++i) {
            for (llu j = 0; j < allInput[i].size(); ++j) {
                text.emplace_back(allInput[i][j], i, j + 1);
            }
        }

    } else if (lastPatternIndex != -1 && allInput.size()) {

        for (llu i = 0; i <= lastPatternIndex; ++i) {
            AddPatternToTrie(allInput[i]);
        }

        for (llu i = lastPatternIndex + 1; i < allInput.size(); ++i) {
            for (llu j = 0; j < allInput[i].size(); ++j) {
                text.emplace_back(allInput[i][j], i, j + 1);
            }
        }

    }

    bool flag = false;

    if (Patterns.size() > 1) {
        flag = true;
    }

    /*for (int i = 0; i < text.size(); ++i) {
        std::cout << text[i].Value << ' ' << text[i].LineNumber << ' ' << text[i].NumberInLine << std::endl;
    }*/

    FindAllPos(text, flag);

    return 0;
}