#include<cstring>
#include<string>
#include<iostream>

/*
* When a lower-tier spiral comes back to root, the number of iterations on root depends on matchLength value at that moment.
* If matchLength is 0, then only 1 iteration on root, create a new leaf node.
* If matchLength is greater than 0, say for example 5, then their will be 6 iterations on root.
* (1). in each of the first 5 iterations, I need to break an edge, create a new intermediate and a new leaf off that new intermediate
* (2). in the last iteration, I need to create a new leaf off root
*
* Root itself can be viewed as a tier. A root-self-spiral is no different than lower-tier-spiral.
* In a root-self-spiral, this can be a (1) only-adding-new-leaf scenario or (2) need-to-break-edges scenario.
* In case (1), there was no match before, current character is also unique,
* just add 1 new leaf off root, then I'm done.
* In case (2), there is a match before but I remain standing on root, current character deviates.
* So I need to break the edge, keep spinning on root, each iteration has a matchLength 1 character shorter than previous iteration.
* Eventually last iteration adds a new leaf off root.
*/
namespace Ukkonen_Suffix_Tree_Lyk {

    using namespace std;

    struct TreeNode
    {
        struct TreeNode* children[26];
        struct TreeNode* sibling;
        int start;
        int end;
        bool isLeaf;
        int suffixStartingIndex;

        TreeNode() :
            sibling(nullptr),
            start(-1),
            end(-1),
            isLeaf(false),
            suffixStartingIndex(0)
        {
            memset(children, 0, sizeof(children));
        }
    };

    class UkkSuffixTree
    {
    private:

        string input;
        TreeNode root;

        TreeNode* currentNode;
        int probe; // currentChildIndexInChildrenArray
        int matchLength;
        int extensionToDoCount;
        TreeNode* lastNewIntermediateNode;
        int leafEndIdx; // leaf node ending character's index in original input string

        void buildTree()
        {
            size_t sz = input.size();
            for (int i = 0; i < sz; ++i)
            {
                addOneChar(i);
            }
        }

        int segmentLength(TreeNode* probe)
        {
            if (probe->isLeaf)
            {
                return leafEndIdx - probe->start + 1;
            }
            else
            {
                return probe->end - probe->start + 1;
            }
        }

        void addOneChar(int i)
        {
            lastNewIntermediateNode = nullptr;
            ++extensionToDoCount;
            leafEndIdx = i;

            char c = input[i] - 'a';

            while (extensionToDoCount > 0)
            {
                if (matchLength == 0)
                {
                    if (currentNode->children[c] == nullptr)
                    {
                        TreeNode* newLeaf = new TreeNode();

                        newLeaf->start = i;

                        newLeaf->isLeaf = true;
                        // 0, 1, 2, 3, 4, 5, 6
                        //                   |
                        //                   i = 6
                        //                |<-->|
                        // extensionToDoCount = 2;
                        // the suffix string I'm dealing with right now
                        // should be the one starting at 5
                        newLeaf->suffixStartingIndex = i - extensionToDoCount + 1;

                        currentNode->children[c] = newLeaf;
                        --extensionToDoCount;

                        if (lastNewIntermediateNode != nullptr)
                        {
                            // I'm adding a new leaf and last new intermediate node is not nullptr,
                            // this must be a lower-tier-spiral-coming-back-to-root scenario.
                            // Generally speaking, a lower-tier-spiral case can (1) only add new leaf or (2) add new intermediate and new leaf
                            // If it's case (1), then "lastNewIntermediate" is never set, I don't need to worry about sibling links
                            // If it's case (2), I have operated on root at least one (potentially multiple times), 
                            // previous operations on root all broke an edge and created new intermediate nodes. 
                            // This is the last operation on root, also the last of this phase, I only create a new leaf, no new intermediate,
                            // but the new intermediate created in last iteration must be linked to root so that a "tier" is fully linked.
                            lastNewIntermediateNode->sibling = currentNode;
                        }
                    }
                    else
                    {
                        probe = c;
                        matchLength = 1;
                        // need to see if I need to move down to the next tier to set currentNode
                        int underneathSegmentLength = segmentLength(currentNode->children[probe]);
                        if (underneathSegmentLength == matchLength)
                        {
                            currentNode = currentNode->children[probe];
                            matchLength = 0;
                            probe = -1;
                        }
                        break;
                    }
                }
                else
                {
                    TreeNode* n = currentNode->children[probe];
                    char nextCharInPath = input[n->start + matchLength];
                    if (nextCharInPath == input[i])
                    {
                        ++matchLength;
                        int underneathSegmentLength = segmentLength(n);
                        if (underneathSegmentLength == matchLength)
                        {
                            currentNode = n;
                            matchLength = 0;
                            probe = -1;
                        }
                        break;
                    }
                    else
                    {
                        TreeNode* newIntermediate = new TreeNode();
                        newIntermediate->start = n->start;
                        n->start = n->start + matchLength;
                        newIntermediate->end = n->start - 1;
                        newIntermediate->isLeaf = false;

                        TreeNode* newLeaf = new TreeNode();
                        newLeaf->start = i;
                        newLeaf->isLeaf = true;
                        newLeaf->suffixStartingIndex = i - extensionToDoCount + 1;
                        //      current node
                        //          |
                        //          e
                        //          t
                        //          w
                        //          |
                        //     an intermediate
                        //
                        //   probe = 4     a  b  c  d  e
                        //                 0  1  2  3  4
                        //
                        // matching 'e' and 't', don't match 'w'
                        //
                        //       current node
                        //          |
                        //          e
                        //          t
                        //          |
                        //        new intermediate
                        //          |          \
                        //          w           x
                        //          |            \
                        //    old intermediate    new leaf
                        //
                        //  nextCharInPath is 'w'
                        //  input[i] is 'x'
                        //
                        currentNode->children[probe] = newIntermediate;

                        newIntermediate->children[nextCharInPath - 'a'] = n;
                        newIntermediate->children[c] = newLeaf;

                        if (lastNewIntermediateNode != nullptr)
                        {
                            lastNewIntermediateNode->sibling = newIntermediate;
                        }
                        lastNewIntermediateNode = newIntermediate;
                    }
                    --extensionToDoCount;
                }

                if (extensionToDoCount == 0)
                {
                    // this is already the last extention for this phase.
                    // I should be on root. This phase didn't end early because of a match.
                    // I added new leaves for the last a few extensions.
                    // reset probe and match length
                    probe = -1;
                    matchLength = 0; // in fact, matchLength right now should already be 0
                }
                else
                {
                    // this is either a pure-root-self-spiral scenario,
                    // or a lower-tier-spiral-but-coming-back-to-root

                    if (currentNode == &root)
                    {
                        if (matchLength > 1)
                        {
                            // need to keep spinning on root
                            char nextCharInPath = input[i - extensionToDoCount + 1];
                            probe = nextCharInPath - 'a';
                        }
                        // if matchLength is 1, I will be adding a new leaf off root in the next iteration,
                        // probe won't mater any more, as long as matchLength is decremented to 0.
                        --matchLength;
                    }
                    else
                    {
                        currentNode = currentNode->sibling;
                    }
                }
            }
        }


        void dfsTraverse1(TreeNode* n)
        {
            if (n->start == -1 && n->end == -1)
            {
                cout << "root" << endl;
            }
            else if (n->isLeaf)
            {
                cout << "leaf:   suffix substring starting index: " << n->suffixStartingIndex << ", end: " << leafEndIdx << endl;
            }
            else
            {
                cout << "intermediate:   start: " << n->start << ", end: " << n->end << endl;
            }
            for (int i = 0; i < 26; ++i)
            {
                if (n->children[i] != nullptr)
                {
                    dfsTraverse1(n->children[i]);
                }
            }
        }

        void dfsTraverse2(TreeNode* n)
        {
            if (n->isLeaf)
            {
                //cout<<"delete a leaf, suffix substring starting index: " << n->suffixStartingIndex << endl;
                delete n;
            }
            else
            {
                for (int i = 0; i < 26; ++i)
                {
                    if (n->children[i] != nullptr)
                    {
                        dfsTraverse2(n->children[i]);
                    }
                }
                if (!(n->start == -1 && n->end == -1))
                {
                    //cout<<"delete an intermediate, start: " <<n->start << ", end: " << n->end << endl;
                    delete n;
                }
            }
        }

    public:

        UkkSuffixTree(const char* inputString) :
            input(inputString),
            root(),
            currentNode(&root),
            probe(-1),
            matchLength(0),
            extensionToDoCount(0),
            lastNewIntermediateNode(nullptr),
            leafEndIdx(0)
        {
            buildTree();
        }

        UkkSuffixTree(string const& s) :
            input(s),
            root(),
            currentNode(&root),
            probe(-1),
            matchLength(0),
            extensionToDoCount(0),
            lastNewIntermediateNode(nullptr),
            leafEndIdx(0)
        {
            buildTree();
        }

        UkkSuffixTree(string&& s) :
            input(std::move(s)),
            root(),
            currentNode(&root),
            probe(-1),
            matchLength(0),
            extensionToDoCount(0),
            lastNewIntermediateNode(nullptr),
            leafEndIdx(0)
        {
            buildTree();
        }

        void printTree()
        {
            //cout<<endl<<"==================================="<<endl;
            dfsTraverse1(&root);
        }


        void dfsTraverse3(TreeNode* n, int& end, int& maxLength, int accumulatedLength)
        {
            int currentNodeLength;
            if (n->start == -1 && n->end == -1)
            {
                currentNodeLength = 0; // n is root
            }
            else
            {
                currentNodeLength = n->end - n->start + 1;
            }
            if (currentNodeLength + accumulatedLength > maxLength)
            {
                maxLength = currentNodeLength + accumulatedLength;
                end = n->end;
            }
            for (int i = 0; i < 26; ++i)
            {
                TreeNode* child = n->children[i];
                if (child != nullptr && !(child->isLeaf))
                {
                    dfsTraverse3(child, end, maxLength, accumulatedLength + currentNodeLength);
                }
            }
        }

        string findLongestDuplicateSubstring()
        {
            int maxLength = 0;
            int end = 0;

            dfsTraverse3(&root, end, maxLength, 0);
            if (maxLength > 0)
            {
                int start = end - maxLength + 1;
                return string{ input.substr(start, maxLength) };
            }
            else
            {
                return "";
            }
        }

        ~UkkSuffixTree()
        {
            cout << endl << "===================================" << endl;
            dfsTraverse2(&root);
        }
    };

    void Test_Ukkonen_Suffix_Tree_Lyk()
    {
        string s;
        while (true)
        {
            cout << "input: ";
            getline(cin, s);
            if (s.size() == 0)
                break;
            cout << "got input: " << s << endl;
            Ukkonen_Suffix_Tree_Lyk::UkkSuffixTree uksuft{ s };
            cout << "longest duplicate substring: " << uksuft.findLongestDuplicateSubstring() << endl;
            s.clear();
        }
    }
}