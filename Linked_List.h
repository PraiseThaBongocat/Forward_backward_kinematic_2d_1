#include <cmath>
#pragma once


template <typename T>
struct Node{
	T data;
	Node<T>* next;
};

template <typename U>
class Array{
	public:
	Node<U>* _head = new Node<U>{NULL,nullptr};
	//Node<U>* _head = new Node{first, nullptr};
	//_head.next = _head;
	Array(U firsts = NULL){
		_head -> data = firsts;
		_head -> next = _head;
	}
	
	Node<U>* Lasts(){
		Node<U>* _current = _head ->next;
		while (_current ->next != _head){
			_current = _current -> next;
		}
		return _current;
	}
	
	bool Is_empty(){
		return _head -> next == _head;
	}
	
	void Add(U _next){
			Node<U>* newNode = new Node<U>{_next, _head};
			Node<U>* lastNode = Lasts();
			lastNode -> next = newNode;
	}
	
};
