#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_


#include "cocos2d.h"
#include "KEvent.h"

class Block;
class BlockClearEventArg
{
public:
	BlockClearEventArg()
	{

	}
	~BlockClearEventArg()
	{

	}
	std::vector<Block*> blocks;
};

class ChessBoard :public cocos2d::Sprite
{
	static const int DefaultColumnCount = 6;
	static const int DefaultRowCount = 6;
	static const int DefaultMarginP = 2;
	static const int DefaultBoarderP = 20;
public:
	static ChessBoard* create(int, int);
	const cocos2d::Size& getBlockSize()
	{
		return m_blockSize;
	}
	virtual void setContentSize(const cocos2d::Size& var) override;
	virtual void setBlockSize(const cocos2d::Size& var);
	virtual void start();
	virtual void destroy(int i, float animetime);
	virtual void destroyAll(float animetime);
	virtual void refreshBackGround();
	virtual void trySwap(int index_1, int index_2)
	{
		swapBlock(index_1, index_2, cocos2d::CallFunc::create(std::bind(&ChessBoard::afterSwap, this, index_1, index_2)));
	}
	virtual bool checkSwap(int index_1, int index_2);
	virtual int checkSwap(int index_1, int index_2, int* cfs);
	virtual void pressBlock(int index);
	virtual void lockBlock(int index,float time);
	virtual bool isLocked(int index);
	virtual bool isNULL(int index);
	virtual void allowHelp(float time);
	virtual void banHelp();
	KEvent<ChessBoard,BlockClearEventArg> blockClearEvent;
	virtual int getBlockType(int index);
	virtual int getTotalBlockCount()
	{
		return m_totalBlockCount;
	}
	virtual int getRowCount()
	{
		return m_rowCount;
	}
	virtual int getColCount()
	{
		return m_columnCount;
	}
	int pointToIndex(const int& x, const int& y) const
	{
		return x + y*m_columnCount;
	}
	cocos2d::Point indexToPoint(const int& index) const
	{
		return cocos2d::Point(index%m_columnCount, (int)(index / m_columnCount));
	}
	virtual cocos2d::Point pointToClient(const float& x, const float& y) const;
	virtual cocos2d::Point indexToClient(const int index) const
	{
		return pointToClient(index%m_columnCount, (int)(index / m_columnCount));
	}
	virtual bool onTouchBegan(cocos2d::Point location);
	virtual void onTouchMoved(cocos2d::Point location);
	Block** Blocks;
protected:
	int m_columnCount;
	int m_rowCount;
	int m_totalBlockCount;
	double m_Margin;
	double m_Border;
	int m_MarginP;
	int m_BorderP;
	int t_beginX;
	int t_beginY;
	bool m_roadChecked;
	bool m_help;
	float m_helpTime;
	cocos2d::Point t_beginPoint;
	cocos2d::Size m_blockSize;
	cocos2d::ParticleBatchNode* explosionnode;
	cocos2d::DrawNode* backgroundnode;
	cocos2d::Node* boardernode;
	cocos2d::Sprite* selectSprite;
	cocos2d::Sprite* helpSprite;
	ChessBoard(int row, int col) :m_columnCount(col), m_rowCount(row),
		m_Margin(0), m_Border(0), m_blockSize(0, 0), m_MarginP(DefaultMarginP), m_BorderP(DefaultBoarderP),
		Blocks(0), t_beginX(-1), t_beginY(-1), m_totalBlockCount(0), selectSprite(0), m_roadChecked(false),
		m_help(false), m_helpTime(3), helpSprite(0), blockClearEvent(this)
	{
	}
	virtual bool init() override;
	virtual void addBlockWithDrop(Block* target, int x, int y);
	virtual Block* newBlock(int type = -1);
	virtual void swapBlock(int index_1, int index_2, cocos2d::CallFunc* func = NULL);
	virtual void afterSwap(int index_1, int index_2);
	virtual bool autoCheck();
	virtual void fillBoard();
	virtual bool hasRoad();
	virtual bool check(Block* target, std::vector<Block*>& list);
	virtual void checkRow(Block* target, std::vector<Block*>& list);
	virtual void checkCol(Block* target, std::vector<Block*>& list);
	virtual void clearList(std::vector<Block*>& list);
	virtual void select(int x, int y);
	virtual void unselect();
	virtual void help(int index_1, int index_2);
	virtual void cancelhelp();
	virtual void schdulehelp(int index_1, int index_2, float dt);
	virtual void update(float dt) override;
};

#endif