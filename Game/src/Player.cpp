#include "canpch.h"
#include "Player.h"
#include "GameLayer.h"

Player::Player(int width, int height, NeuralNetwork* brain, GameLayer* parent)
	: m_Width(width)
	, m_Height(height)
	, m_Brain(brain)
	, m_Parent(parent)
	, m_CurrentX(m_Width / 2)
	, m_CurrentY(m_Height - 1)
	, m_CurrentBlockIndex(Can::Utility::Random::Integer(7))
	, m_CurrentBlock(m_Parent->GetBlock(m_CurrentBlockIndex))
{
	for (int i = 0; i < m_Height; i++)
	{
		m_State.push_back({});

		for (int j = 0; j < m_Width; j++)
			m_State[i].push_back(false);

	}
	for (int i = 0; i < BLOCK_QUEUE_SIZE; i++)
	{
		m_BlockQueue[i] = Can::Utility::Random::Integer(7);
	}
}

Player::~Player() {

}

void Player::Update(float ts)
{
	IsGameOver();

	if (IsDead())
		return;

	m_Counter++;
	if (m_Counter >= m_MaxCount)
	{
		m_Parent->AddData(GetState());
		if (MoveDown())
		{
			BlockToState();
			NewBlock();
		}
		m_Counter = 0;
		return;
	}

	int result = Think();

	if (result == 1)
		Rotate();				// Rotate
	else if (result == 2)
		MoveBlockDown();		// Down
	else if (result == 3)
		MoveHorizontal(true);	// Left
	else if (result == 4)
		MoveHorizontal(false);	// Right
}

void Player::Draw(const glm::vec2 & offset)
{
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			Can::Renderer2D::DrawQuad(
				{ offset.x + j, offset.y + i },
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);
			if (m_State[i][j])
				Can::Renderer2D::DrawQuad(
					{ offset.x + j,offset.y + i, 0.01f },
					{ 0.9f, 0.9f },
					{ 0.3f, 0.8f, 0.2f, 1.0f }
			);
			else
				Can::Renderer2D::DrawQuad(
					{ offset.x + j, offset.y + i, 0.01f },
					{ 0.9f, 0.9f },
					{ 0.9f, 0.9f, 0.9f, 1.0f }
			);
		}
	}

	if (!IsDead())
		for (int i = 0; i < m_CurrentBlock.size(); i++)
		{
			for (int j = 0; j < m_CurrentBlock[0].size(); j++)
			{
				if (m_CurrentBlock[i][j])
					Can::Renderer2D::DrawQuad(
						{ offset.x + j + m_CurrentX, offset.y - i + m_CurrentY, 0.011f },
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
			}
		}

	if (true)
	{
		std::vector<std::vector<bool>> block;
		glm::vec2 pos = { m_Width + 2, (m_Height * 1.0f) / 2.0f - 1.0f };
		block = m_Parent->GetBlock(m_BlockQueue[(0 + m_BlockIndex) % BLOCK_QUEUE_SIZE]);
		for (int i = 0; i < block.size(); i++)
		{
			for (int j = 0; j < block[0].size(); j++)
			{
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y
					},
					{ 1.0f, 1.0f },
					{ 0.05f, 0.05f, 0.05f, 1.0f }
				);

				if (block[i][j])
					Can::Renderer2D::DrawQuad(
						{
							offset.x + j + pos.x,
							offset.y - i + pos.y,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else
					Can::Renderer2D::DrawQuad(
						{
							offset.x + j + pos.x,
							offset.y - i + pos.y,
							0.011f },
						{ 0.9f, 0.9f },
						{ 0.9f, 0.9f, 0.9f, 1.0f }
				);
			}
		}
		pos = { m_Width + 2, (m_Height * 2.0f) / 2.0f - 2.0f };
		block = m_Parent->GetBlock(m_BlockQueue[(1 + m_BlockIndex) % BLOCK_QUEUE_SIZE]);
		for (int i = 0; i < block.size(); i++)
		{
			for (int j = 0; j < block[0].size(); j++)
			{
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y
					},
					{ 1.0f, 1.0f },
					{ 0.05f, 0.05f, 0.05f, 1.0f }
				);

				if (block[i][j])
					Can::Renderer2D::DrawQuad(
						{
							offset.x + j + pos.x,
							offset.y - i + pos.y,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else
					Can::Renderer2D::DrawQuad(
						{
							offset.x + j + pos.x,
							offset.y - i + pos.y,
							0.011f },
						{ 0.9f, 0.9f },
						{ 0.9f, 0.9f, 0.9f, 1.0f }
				);
			}
		}
		
	}
}

void Player::MoveBlockDown()
{
	m_Parent->AddData(GetState());
	while (!MoveDown());
	m_Counter = 0;
	BlockToState();
	NewBlock();
}

bool Player::WillCrush(const glm::vec2 & direction)
{
	int height = m_CurrentBlock.size();
	for (size_t i = 0; i < height; i++)
	{
		int width = m_CurrentBlock[i].size();
		for (size_t j = 0; j < width; j++)
		{
			if (j + m_CurrentX + direction.x == -1)
				return true;
			if (j + m_CurrentX + direction.x == m_Width)
				return true;
			if (-i + m_CurrentY + direction.y == -1)
				return true;
			if (m_CurrentBlock[i][j] && m_State[-i + (size_t)direction.y + (size_t)m_CurrentY][j + (size_t)direction.x + (size_t)m_CurrentX])
				return true;
		}
	}

	return false;
}

void Player::MoveHorizontal(bool isLeft)
{
	if ((isLeft) ? WillCrush({ -1, 0 }) : WillCrush({ 1, 0 }))
		return;
	m_Parent->AddData(GetState());
	m_CurrentX -= isLeft;
	m_CurrentX += !isLeft;
}

void Player::NewBlock()
{
	m_CurrentX = m_Width / 2;
	m_CurrentY = m_Height - 1;
	m_CurrentBlockIndex = m_BlockQueue[m_BlockIndex];
	m_CurrentBlock = m_Parent->GetBlock(m_CurrentBlockIndex);
	m_BlockQueue[m_BlockIndex] = Can::Utility::Random::Integer(7);
	m_BlockIndex = (m_BlockIndex + 1) % BLOCK_QUEUE_SIZE;
	m_CurrentBlockRotation = 0;
}

bool Player::MoveDown()
{
	if (WillCrush({ 0, -1 }))
		return true;

	m_Parent->AddData(GetState());
	m_CurrentY--;
	return false;
}

int Player::Think()
{
	float* state = new float[STATE_SIZE];

	int index = 0;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			state[index] = m_State[i][j];
			index++;
		}
	}
	for (int i = 0; i < BLOCK_QUEUE_SIZE; i++)
	{
		state[index + i] = (m_BlockQueue[(i + m_BlockIndex) % BLOCK_QUEUE_SIZE] + 1.0f) / (4.0f);
	}
	state[index + BLOCK_QUEUE_SIZE + 0] = (m_CurrentBlockIndex + 1.0f) / (8.0f);
	state[index + BLOCK_QUEUE_SIZE + 1] = (m_CurrentX + 1.0f) / (m_Width + 1.0f);
	state[index + BLOCK_QUEUE_SIZE + 2] = (m_CurrentY + 1.0f) / (m_Height + 1.0f);
	state[index + BLOCK_QUEUE_SIZE + 3] = (m_CurrentBlockRotation + 1.0f) / (4.0f);

	Matrix * input = new Matrix(STATE_SIZE, 1, state);
	Matrix * result = m_Brain->FeedForward(input);

	float A[5] = {
		result->data[0],	// Don't Do anything
		result->data[1],	// Rotate
		result->data[2],	// Down
		result->data[3],	// Left
		result->data[4]		// Right
	};
	float* maxElement = std::max_element(A, A + 5);
	_int64 dist = std::distance(A, maxElement);

	delete result;

	return dist;
	//return 0;
}

void Player::Rotate()
{
	std::vector<std::vector<bool>> temp;
	for (int j = 0; j < m_CurrentBlock[0].size(); j++)
	{
		temp.push_back({});
		for (int i = m_CurrentBlock.size() - 1; i >= 0; i--)
		{
			temp[j].push_back(m_CurrentBlock[i][j]);
		}
	}
	int tempX = m_CurrentX;

	if (m_CurrentBlock.size() + tempX > m_Width)
	{
		tempX = m_Width - m_CurrentBlock.size();
	}

	bool available = true;
	for (int i = 0; i < m_CurrentBlock[0].size(); i++)
	{
		for (int j = 0; j < m_CurrentBlock.size(); j++)
		{
			if (-i + m_CurrentY < 0 || (temp[i][j] && m_State[-i + (size_t)m_CurrentY][j + (size_t)tempX]))
				available = false;
		}
	}
	if (available)
	{
		m_Parent->AddData(GetState());
		m_CurrentX = tempX;
		m_CurrentBlock.clear();
		for (int j = 0; j < temp.size(); j++)
		{
			m_CurrentBlock.push_back({});
			for (int i = 0; i < temp[0].size(); i++)
			{
				m_CurrentBlock[j].push_back(temp[j][i]);
			}
		}
		m_CurrentBlockRotation = (m_CurrentBlockRotation + 1) % 4;
	}
}

void Player::IsGameOver()
{
	for (int i = 0; i < m_Width; i++)
	{
		if (m_State[(size_t)m_Height - 1][i])
		{
			m_IsDead = true;
			m_Parent->PlayerDied();
			return;
		}
	}
}

void Player::BlockToState()
{
	for (size_t i = 0; i < m_CurrentBlock.size(); i++)
	{
		for (size_t j = 0; j < m_CurrentBlock[0].size(); j++)
			if (-i + m_CurrentY >= 0 && -i + m_CurrentY < m_Height && m_CurrentBlock[i][j])
				m_State[-i + (size_t)m_CurrentY][j + (size_t)m_CurrentX] = true;
	}
	BreakFullRows();
}

void Player::BreakFullRows()
{
	std::vector<std::vector<bool>> temp;
	int count = 0;
	for (int i = m_Height - 1; i >= 0; i--)
	{
		bool full = true;
		for (int j = 0; j < m_Width; j++)
		{
			if (m_State[i][j] == false)
			{
				temp.push_back(m_State[i]);
				full = false;
				break;
			}
		}
		if (full)
			count++;
	}
	for (int i = 0; i < count; i++)
	{
		std::vector<bool> row;
		for (int j = 0; j < m_Width; j++)
		{
			row.push_back(false);
		}
		temp.insert(temp.begin(), row);
	}
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			m_State[i][j] = temp[(size_t)m_Height - 1 - i][j];
		}
	}
	m_Point += count * count * 10;
}

std::array<float, STATE_SIZE> Player::GetState()
{
	std::array<float, STATE_SIZE> result;
	size_t index = 0;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			result[index] = m_State[i][j];
			index++;
		}
	}
	for (int i = 0; i < BLOCK_QUEUE_SIZE; i++)
	{
		result[index + i] = m_BlockQueue[(i + m_BlockIndex) % BLOCK_QUEUE_SIZE];
	}
	result[index + BLOCK_QUEUE_SIZE + 0] = m_CurrentBlockIndex;
	result[index + BLOCK_QUEUE_SIZE + 1] = m_CurrentX;
	result[index + BLOCK_QUEUE_SIZE + 2] = m_CurrentY;
	result[index + BLOCK_QUEUE_SIZE + 3] = m_CurrentBlockRotation;
	return result;
}