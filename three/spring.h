#ifndef SPRING_H
#define SPRING_H

class Spring {
public:
	//should be able to access its two nodes
	int node1;
	int node2;
	float rest_length;
	Spring(int n1, int n2, float rl);
};
#endif