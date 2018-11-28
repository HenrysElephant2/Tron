#include "TransparentRenderer.h"

// Transparent::Transparent() {}
Transparent::~Transparent(){}

TLL::TLL( Transparent* o, double d ) {
	ob = o;
	dist = d;
	next = NULL;
}

TLL::~TLL() {
	if( next != NULL )
		delete next;
}

TransparentRenderer::TransparentRenderer() {
	start = NULL;
	end = NULL;
	num = 0;
}

TransparentRenderer::~TransparentRenderer() {
	delete start;
}

void TransparentRenderer::add( Transparent *toAdd, double dist ) {
	if( start == NULL ) {
		start = new TLL(toAdd, dist);
		end = start;
	} 
	else {
		end->next = new TLL(toAdd, dist);
		end = end->next;
	}
	num++;
}

void TransparentRenderer::display() {
	Transparent *obs[num];
	double dists[num];

	TLL *cur = start;
	for( int i=0; i<num; i++ ) {
		obs[i] = cur->ob;
		dists[i] = cur->dist;
		cur = cur->next;
	}

	MergeSort(obs, dists, 0, num-1);


	glDepthMask(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

	for( int i=0; i<num; i++ ) {
		obs[i]->display();
	}

	glDisable(GL_BLEND);
	glDepthMask(1);
}

void TransparentRenderer::Merge( Transparent *obs[], double dists[], int low, int high, int mid ) {
	double tempD[high-low+1];
	Transparent* tempObs[high-low+1];

	int i = low;
	int j = mid+1;
	int k = 0;
 
	while( i <= mid && j <= high ) {
		if( dists[i] < dists[j] ) {
			tempD[k] = dists[i];
			tempObs[k] = obs[i];
			i++;
		}
		else {
			tempD[k] = dists[j];
			tempObs[k] = obs[j];
			j++;
		}
		k++;
	}

	while( i <= mid ) {
		tempD[k] = dists[i];
		tempObs[k] = obs[i];
		k++;
		i++;
	}
	while( j <= high ) {
		tempD[k] = dists[j];
		tempObs[k] = obs[j];
		k++;
		j++;
	}

	for( i=low; i<=high; i++ ) {
		dists[i] = tempD[i-low];
		obs[i] = tempObs[i-low];
	}
}
 
void TransparentRenderer::MergeSort( Transparent *obs[], double dists[], int low, int high ) {
	int mid;
	if( low < high ) {
		mid = (low+high)/2;
		MergeSort( obs, dists, low, mid );
		MergeSort( obs, dists, mid+1, high );
 		
		Merge( obs, dists, low, high, mid );
	}
}