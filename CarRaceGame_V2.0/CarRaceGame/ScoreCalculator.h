#pragma once
class ScoreCalculator final
{
public:
	explicit ScoreCalculator();

	void Draw() const;
	void Update(float elapsedSec);

	int GetScore() const;

private:
	int m_Score;
	float m_AccuSec;
};

