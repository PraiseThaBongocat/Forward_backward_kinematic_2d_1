#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include "Linked_List.h"

float pi = 3.14159265;
float screen_x = 2800;
float screen_y = 1575;

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;

void init(){
	window = SDL_CreateWindow("bwa", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_x, screen_y, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
}

class point{
public:
float x;
float y;
point(float x0=0, float y0 = 0){
	x = x0;
	y = y0;
	}	
};

class zven{
public:
	point start, end;
	float a, q;
	bool post;
	zven(point start0 = {screen_x/2, screen_y}, point end0 = {screen_x/2, screen_y - 100}, float a0 = 100, float q0 = 0){
		start = start0;
		end = end0;
		a = a0;
		q = q0;
	}
	
};


class robot{
public:
	float x,y;
	zven first, second;
	float i = 0;
	int model = 1;
	int color = 255;
	point speed;
	float dq1, dq2;
	robot(int model0 = 1, float q01 = 0, float q02 = 0, float a02 = 100, float a01 = 100, float dq01 = 0, float dq02 = 0, point speed0 = {0,0}, int color0 = 255){
		model = model0;
		first.a = a01;
		first.q = q01;
		second.a = a02;
		second.q = q02;
		x = screen_x/2;
		y = screen_y;
		dq1 = dq01;
		dq2 = dq02;
		speed = speed0;
		color = color0;
	}
	
	void Change(point p){
		//решение всех четырёх обратных кинематических задач в виде требуемых dq1, dq2.
		if (i == 0){
			float test = 0;
			switch (model){
				case 1:
			    if(p.y >= y - first.a){
			    	dq1 = (-first.q)/100;
			    	color = 0;
			    }else{
				    dq1 = (-p.y + y - first.q - first.a)/100;
				    color = 255;
			    }
				dq2 = (p.x - x - second.q-second.a)/100;
				i+=1;
				break;
				case 2:
				dq1 = (atan2(p.x - x, y - p.y) - first.q)/100;
				dq2 = (pow((p.x - x)*(p.x - x) + (y - p.y)*(y - p.y), 0.5) - second.a - second.q)/100;
				i+=1;
				break;
				case 3:
				test = (p.x - x)/(second.a);
				if (test <1 and test > -1){
					test = asin((p.x - x)/(second.a));
					dq2 = (asin((p.x - x)/(second.a)) - second.q)/100;
					color = 255;
				}else{
					test = pi/2*(test/pow(test*test, 0.5));
					dq2 =  (test- second.q)/100;
					color = 0;
				}
				dq1 = (y-p.y - first.a - second.a * cos(test) - first.q)/100;
				//std::cout <<"q: " << dq1*100 << ", " << dq2*100 << "] " << test << std::endl;
				i+=1;
				break;
				case 4:
				test = pow((p.x - x) * (p.x - x) + (y - p.y)*(y - p.y), 0.5);
				if(test < second.a + first.a and test>pow(pow(second.a - first.a, 2), 0.5)){
					dq2 = acos((test*test - first.a*first.a - second.a*second.a)/(2*second.a*first.a));
					dq1 = atan2((p.x - x), (y-p.y)) - atan2( second.a * sin(dq2),  first.a + second.a*cos(dq2) );
					dq2 = (dq2 - second.q)/100;
					dq1 = (dq1 - first.q)/100;
					color = 255;
				}else {
					dq1 = atan2((p.x - x), (y-p.y));
					dq2 = - second.q/100;
					dq1 = (dq1 - first.q)/100;
					color = 20;
				}
				std::cout <<"q: " << dq1*100 << ", " << dq2*100 << "] " << test << std::endl;
				i+=1;
				break;	
			}
		}else if(i<=100){
				first.q += dq1;
				second.q += dq2;
				i+=1;
		}else{
			dq1 =0;
			dq2 = 0;
		}
	}
	
	void Draw(int color1 = 255){
		SDL_SetRenderDrawColor(render, color1, color, color, 255);
		SDL_Rect temp0 = {int(x-10), int(y-10), 20,20};
		//решение всех 4рёх прямых задач, отрисовка робота по его q1, q2
		switch(model){
			case 1:
			
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x, y-10, x,y - first.q - first.a);
			SDL_RenderDrawLine(render, x-10, y-10, x-10,y - first.q - first.a);
			SDL_RenderDrawLine(render, x+10, y-10, x+10,y - first.q - first.a);
			
			temp0 = {int(x - 10), int(y - first.q - first.a - 10), 20, 20 };
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x, y - first.q - first.a, x+second.q+second.a,  y - first.q - first.a );
			SDL_RenderDrawLine(render, x, y+10 - first.q - first.a, x+second.q+second.a,  y+10 - first.q - first.a );
			SDL_RenderDrawLine(render, x, y-10 - first.q - first.a, x+second.q+second.a,  y-10 - first.q - first.a );
			
			temp0 = {int(x - 5 +second.q+second.a), int( y-5 - first.q - first.a ), 10, 10 };
			SDL_RenderFillRect(render, &temp0);
			break;
			case 2:
			
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x, y-10, x+cos(first.q - pi/2)*(second.q + second.a), y - sin(first.q+pi/2)*(second.q + second.a));
			SDL_RenderDrawLine(render, x-5, y-10, x+cos(first.q - pi/2)*(second.q + second.a), y - sin(first.q+pi/2)*(second.q + second.a));
			SDL_RenderDrawLine(render, x+5, y-10, x+cos(first.q - pi/2)*(second.q + second.a), y - sin(first.q+pi/2)*(second.q + second.a));
			
			
			
			temp0 = {int(x - 5+cos(first.q - pi/2)*(second.q + second.a)), int(y - 5 - sin(first.q + pi/2)*(second.q + second.a)), 10, 10 };
			SDL_RenderFillRect(render, &temp0);
			break;
			case 3:
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x, y, x,y - first.q - first.a);
			
			temp0 = {int(x - 10), int(y -10 - first.q - first.a - 10), 20, 20 };
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x, y - first.q - first.a, x + second.a*sin(second.q), y - first.q - first.a - second.a*cos(second.q));
			
			
			temp0 = {int(x + second.a*sin(second.q)), int( y - first.a - first.q - second.a*cos(second.q)), 10, 10 };
			SDL_RenderFillRect(render, &temp0);
			break;
			case 4:
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x, y, x + first.a*sin(first.q),y - first.a*cos(first.q));
			
			temp0 = {int(x + first.a*sin(first.q) - 10),int(y - first.a*cos(first.q) -10), 20, 20 };
			SDL_RenderFillRect(render, &temp0);
			
			SDL_RenderDrawLine(render, x + first.a*sin(first.q),y - first.a*cos(first.q),  x + first.a*sin(first.q) + second.a*sin(second.q+first.q),y - first.a*cos(first.q) - second.a*cos(first.q + second.q));
			
			
			temp0 = { int(x-5 + first.a*sin(first.q) + second.a*sin(second.q+first.q)),int(y - 5 - first.a*cos(first.q) - second.a*cos(first.q + second.q)), 10, 10 };
			SDL_RenderFillRect(render, &temp0);
			break;
			
			
		}
	}

};

/***
void Change(point* p, robot* r, int* i){
		if(*i == 0){
			float radius = pow((p->x)*(p->x)+(p->y)*(p->y), 0.5);
			float dx = p->x - r->x;
			float dy = p->y - r->y;
			dq2 = atan2f(dx, -dy) * 180.0 / pi - r->q2;  
			float distance = pow((p->x - r-> x)*(p->x - r-> x) +(p->y - r-> y)*(p->y - r-> y),0.5);
			dq1 = distance - r->a2*100 - r->q1;
			(*i)++;
		}else if(*i <=100){
		(*i)++;
		r->q1 += dq1/100;
		r->q2 += dq2/100;
		}
	}***/

void Quit(){
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	
}

/***
void DRAW(point* p, robot* r){
	SDL_SetRenderDrawColor(render, 100,255,255,255);
	
	SDL_Rect point0 = {int(p->x-5), int(p->y-5), 10,10};
	SDL_RenderFillRect(render, &point0);
	
	SDL_SetRenderDrawColor(render, 255,255,255,255);
	float distance = pow((p->x - r-> x)*(p->x - r-> x) +(p->y - r-> y)*(p->y - r-> y),0.5);
	if(distance< (r->a2) * 100){
		SDL_SetRenderDrawColor(render, 255,0,0,255);
	}
	float rad = r->q2 * pi/180;
	float endx = (r->x)+(r->a2)*100*sinf(rad);
	float endy = (r->y)-(r->a2)*100*cosf(rad);
	SDL_RenderDrawLine(render, r->x, r->y, endx, endy);
	SDL_Rect rect = {int((r->x)-10), int((r->y)), 20, 10};
	SDL_RenderFillRect(render, &rect);
	
	SDL_SetRenderDrawColor(render, 230, 230, 120, 255);
	SDL_RenderDrawLine(render, endx, endy, endx+(r->q1)*sin(rad), endy - (r->q1)*cos(rad));
	
	
}***/

int main()
{
	init();
	std::cout << SDL_GetError() << std::endl;
	robot R1;
	robot R2;
	robot R3;
	robot R4;
	Array<robot> A(R1);
	R2.model = 2;
	R3.model = 3;
	R3.second.a = 200;
	R3.first.a = 100;
	R4.model = 4;
	R4.second.a = 400;
	R4.first.a = 300;
	A.Add(R2);
	A.Add(R3);
	A.Add(R4);
	Node<robot>* current = A._head;

	
	point p;
	p.x = screen_x/2;
	p.y = screen_y/2;
	SDL_Event e;
	bool running = true;
	while(running){
		while(SDL_PollEvent(&e)){
			

			if(e.type == SDL_QUIT){
				running = false;
			}
			
			
			if(e.type == SDL_FINGERDOWN){
				if((0 <= (e.tfinger.x * screen_x) and (e.tfinger.x * screen_x) <=200) and  (0<(e.tfinger.y * screen_y) and (e.tfinger.y * screen_y) < 200)){
					SDL_SetRenderDrawColor(render, 250,50,50,50);
					current = current -> next;
					current -> data.i = 0;
				}else{
				p.x = e.tfinger.x * screen_x;
				p.y = e.tfinger.y * screen_y;
				current -> data.i = 0;
				std::cout << p.x << ", " << p.y << std::endl;
				SDL_SetRenderDrawColor(render, 50,50,50,50);
				}
			}
			 
			//std::cout << e.type << ", ";
		
		}
		SDL_Rect temp0 = {0, 0, 200, 200};
		SDL_RenderFillRect(render, &temp0);
		SDL_SetRenderDrawColor(render, 255,255,255,255);
		SDL_RenderDrawRect(render, &temp0);
		
		//Change(&p, &R, &i);
		current -> data.Change(p);
		current -> data.Draw();
		SDL_RenderPresent(render);
		//printf("%f, %f.  \n", R3.first.q, R3.second.q);
		SDL_SetRenderDrawColor(render, 0,0,0,255);
		SDL_RenderClear(render);
		//DRAW(&p, &R);
		
	}
	Quit();
	SDL_Delay(10);
}