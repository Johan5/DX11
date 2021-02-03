#pragma once


class CInput
{
public:
	CInput() = default;
	CInput( const CInput& Other ) = delete;

	void Initialize();

	void KeyDown(unsigned int InputIndex);
	void KeyUp(unsigned int InputIndex );

	bool IsKeyDown(unsigned int InputIndex );

private:
	bool _Keys[256];
};

