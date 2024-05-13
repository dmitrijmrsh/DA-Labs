#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdint>

using ll = long long;

struct TrieNode {
    std::unordered_map<ll, ll> Children;
    std::unordered_map<ll, ll> AutoMove;
    std::vector<ll> Positions;
    bool IsEndOfWord;
    ll PatternSize;
    ll SuffixLink;
    ll Parent;
    ll ParentValue;
    ll GoodSuffixLink;

    TrieNode() {
        IsEndOfWord = false;
        SuffixLink = -1;
        GoodSuffixLink = -1;
        Parent = 0;
        ParentValue = -1;
        PatternSize = -1;
    }

    TrieNode(ll parent, ll parentValue) {
        IsEndOfWord = false;
        SuffixLink = -1;
        Parent = parent;
        ParentValue = parentValue;
        GoodSuffixLink = -1;
        PatternSize = -1;
    }

    ~TrieNode() = default;
};

struct Triplet {
    ll Value;
    ll LineNumber;
    ll NumberInLine;

    Triplet() = default;
    Triplet(ll value, ll lineNumber, ll numberInLine) {
        Value = value;
        LineNumber = lineNumber;
        NumberInLine = numberInLine;
    }

    ~Triplet() = default;
};

std::vector<struct TrieNode> Trie;

void Init() {
    Trie.push_back(TrieNode());
}

void AddPatternToTrie (std::vector<ll>& vec, ll substrStartPos) {
    ll number = 0;

    for (ll i = 0; i < vec.size(); ++i) {
        if (!Trie[number].Children.count(vec[i])) {
            Trie.push_back(TrieNode(number, vec[i]));
            Trie[number].Children[vec[i]] = Trie.size() - 1;
        }

        number = Trie[number].Children[vec[i]];
    }

    Trie[number].IsEndOfWord = true;
    Trie[number].PatternSize = vec.size();
    Trie[number].Positions.push_back(substrStartPos);
}

std::pair<bool, ll> CheckPatternInTrie (std::vector<ll>& vec) {
    ll number = 0;

    for (ll i = 0; i < vec.size(); ++i) {
        if (!Trie[number].Children.count(vec[i])) {
            return {false, number};
        }

        number = Trie[number].Children[vec[i]];
    }

    return {true, number};
}

ll GetAutoMove(ll v, ll value);

ll GetSuffixLink(ll v) {
    if (Trie[v].SuffixLink == -1) {
        if (v == 0 || Trie[v].Parent == 0) {

            Trie[v].SuffixLink = 0;

        } else {

            Trie[v].SuffixLink = GetAutoMove(GetSuffixLink(Trie[v].Parent), Trie[v].ParentValue);

        }
    }

    return Trie[v].SuffixLink;
}

ll GetAutoMove(ll v, ll value) {
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

ll GetGoodSuffixLink(ll v) {
    if (Trie[v].GoodSuffixLink == -1) {
        ll u = GetSuffixLink(v);

        if (u == 0) {
            Trie[v].GoodSuffixLink = 0;
        } else {
            Trie[v].GoodSuffixLink = (Trie[u].IsEndOfWord) ? u : GetGoodSuffixLink(u);
        }
    }

    return Trie[v].GoodSuffixLink;
}

void Check(ll v, ll index, std::vector<ll>& count) {
    for (ll u = v; u != 0; u = GetGoodSuffixLink(u)) {
        if (Trie[u].IsEndOfWord) {
            for (ll i = 0; i < Trie[u].Positions.size(); ++i) {
                if (index - Trie[u].PatternSize + 1 - Trie[u].Positions[i] >= 0) {
                    ++count[index - Trie[u].PatternSize + 1 - Trie[u].Positions[i]];
                }
            }
        }
    }
}

void FindAllPos(std::vector<Triplet>& text, ll substringCount, ll lostJokers) {
    ll u = 0;
    std::vector<ll> count(text.size());

    for (ll i = 0; i < text.size(); ++i) {
        u = GetAutoMove(u, text[i].Value);
        Check(u, i, count);
    }

    if (!substringCount) {

        for (ll i = 0; i < count.size(); ++i) {
            if (i + lostJokers - 1 < count.size()) {
                std::cout << text[i].LineNumber << ", " << text[i].NumberInLine << '\n';
            }
        }

    } else {

        for (ll i = 0; i < count.size(); ++i) {
            if (count[i] == substringCount && i + lostJokers < count.size()) {
                std::cout << text[i].LineNumber << ", " << text[i].NumberInLine << '\n';
            }
        }

    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    Init();

    std::vector<std::pair<std::vector<ll>, ll>> pattern;
    std::vector<Triplet> text;
    std::string str;

    std::getline(std::cin, str, '\n');

    std::istringstream inputPattern(str);
    std::string helpString;
    std::vector<ll> helpVector;
    ll subStringStartIndex = 0, strIndex = 0, lostJokers = 0;

    while (inputPattern >> helpString) {
        if (helpString == "?") {
            if (helpVector.size()) {
                pattern.push_back({helpVector, subStringStartIndex});
            }

            ++strIndex;
            subStringStartIndex = strIndex;
            ++lostJokers;
            helpVector.clear();
            continue;
        }

        helpVector.push_back(std::stoll(helpString));
        lostJokers = 0;
        ++strIndex;
    }

    if (helpVector.size()) {
        pattern.push_back({helpVector, subStringStartIndex});
    }
        
    ll currentLineNumber = 0;
    while (std::getline(std::cin, str, '\n')) {
        ll currentValueNumber = 0;
        ++currentLineNumber;
        std::istringstream inputLine(str);

        while (inputLine >> helpString) {
            ++currentValueNumber;
            text.push_back({std::stoll(helpString), currentLineNumber, currentValueNumber});
        }
    }

    for (ll i = 0; i < pattern.size(); ++i) {
        std::pair<bool, ll> checkPair = CheckPatternInTrie(pattern[i].first);

        if (checkPair.first) {
            Trie[checkPair.second].Positions.push_back(pattern[i].second);
            continue;
        }

        AddPatternToTrie(pattern[i].first, pattern[i].second);
    }

    FindAllPos(text, pattern.size(), lostJokers);

    return 0;
}