BTree: BTree.o  BTreeDriverDebug.o BTreeNode.o InternalNode.o LeafNode.o 
	g++ -Wall -g -o BTree BTree.o BTreeDriverDebug.o BTreeNode.o InternalNode.o LeafNode.o  

BTree.o: BTree.cpp BTree.h BTreeNode.h LeafNode.h InternalNode.h 
	g++ -Wall -g -c BTree.cpp

BTreeDriverDebug.o: BTreeDriverDebug.cpp BTree.h 
	g++ -Wall -g -c BTreeDriverDebug.cpp

BTreeNode.o: BTreeNode.cpp BTreeNode.h 
	g++ -Wall -g -c BTreeNode.cpp

InternalNode.o: InternalNode.cpp InternalNode.h 
	g++ -Wall -g -c InternalNode.cpp

LeafNode.o: LeafNode.cpp LeafNode.h InternalNode.h QueueAr.h 
	g++ -Wall -g -c LeafNode.cpp


clean:
	rm -f BTree BTree.o BTreeDriverDebug.o BTreeNode.o BTreeShort.o InternalNode.o LeafNode.o  
