#pragma once
class BigButton
{
private:
	int pinIn;
	int pinOut;
	const char* message;
	int lastState;
public:
	BigButton(int pinIn, int pinOut, const char message[]);
	~BigButton();
	void process();
};

