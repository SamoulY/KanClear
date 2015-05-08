#include "ChessBoard.h"
#include "Block.h"

using namespace cocos2d;

ChessBoard* ChessBoard::create(int row,int col)
{
	ChessBoard* gl = new ChessBoard(row,col);
	if (gl&&gl->init())
	{
		gl->autorelease();
	}
	else
	{
		delete(gl);
		gl = nullptr;
	}
	return gl;
}
bool ChessBoard::init()
{
	if (!Sprite::init())
		return false;
	this->addChild(explosionnode = ParticleBatchNode::create("Resources/img/stars.png", m_totalBlockCount), 12);
	backgroundnode = DrawNode::create();
	this->addChild(backgroundnode);
	boardernode = Node::create();
	this->addChild(boardernode,11);
	m_totalBlockCount = m_columnCount*m_rowCount;
	Blocks = new Block*[m_totalBlockCount];
	for (int i = 0; i < m_totalBlockCount; i++)
		Blocks[i] = NULL;

	return true;
}

void ChessBoard::refreshBackGround()
{
	backgroundnode->clear();
	backgroundnode->drawSolidRect(Point(0, 0), Point(_contentSize.width, _contentSize.height), Color4F(Color4B(0, 50, 80, 130)));
	boardernode->removeAllChildren();
	float sc;
	Sprite* tempS = Sprite::createWithSpriteFrameName("border_LT.png");
	tempS->setAnchorPoint(Point(0, 1));
	boardernode->addChild(tempS, 10);
	tempS->setPosition(0, _contentSize.height);
	tempS->setScale(sc = _contentSize.height * 9 / (10 * tempS->getContentSize().height));
	tempS = Sprite::createWithSpriteFrameName("border_RB.png");
	tempS->setAnchorPoint(Point(1, 0));
	boardernode->addChild(tempS, 10);
	tempS->setPosition(_contentSize.width, 0);
	tempS->setScale(sc);
	tempS = Sprite::createWithSpriteFrameName("border_LB.png");
	tempS->setAnchorPoint(Point(0, 0));
	boardernode->addChild(tempS, 10);
	tempS->setScale(sc);
	tempS = Sprite::createWithSpriteFrameName("border_RT.png");
	tempS->setAnchorPoint(Point(1, 1));
	boardernode->addChild(tempS, 10);
	tempS->setPosition(_contentSize.width, _contentSize.height);
	tempS->setScale(sc);
	tempS = Sprite::createWithSpriteFrameName("border_side.png");
	tempS->setAnchorPoint(Point(0, 0.5));
	boardernode->addChild(tempS, 11);
	tempS->setPosition(0, _contentSize.height/2);
	tempS->setScale(sc);
	tempS = Sprite::createWithSpriteFrameName("border_side.png");
	tempS->setAnchorPoint(Point(1, 0.5));
	boardernode->addChild(tempS, 11);
	tempS->setPosition(_contentSize.width, _contentSize.height/2);
	tempS->setScale(sc);
	tempS->setFlippedX(true);
	int i, j, count;
	float temp;
	const int linewidth = 20;
	const int linemargin = 5;
	const int totalline = linewidth + linemargin;
	Color4F linecolor = Color4F(Color4B(212, 212, 212, 255));
	Color4F bordercolor = Color4F(Color4B(146, 52, 19, 255));
	for (i = 0; i < m_rowCount - 1; i++)
	{
		count = (_contentSize.width - m_Border * 2) / totalline;
		for (j = 0; j < count; j++)
		{
			temp = m_Border + (i + 1)*(m_blockSize.height + m_Margin);
			backgroundnode->drawLine(Point(totalline * j + m_Border, temp), Point(totalline * j + linewidth + m_Border, temp), linecolor);
		}
		backgroundnode->drawLine(Point(totalline * j + m_Border, temp), Point(_contentSize.width - m_Border, temp), linecolor);
	}
	for (i = 0; i < m_columnCount - 1; i++)
	{
		count = (_contentSize.height - m_Border * 2) / totalline;
		for (j = 0; j < count; j++)
		{
			temp = m_Border + (i + 1)*(m_blockSize.width + m_Margin);
			backgroundnode->drawLine(Point(temp, totalline * j + m_Border), Point(temp, totalline * j + linewidth + m_Border), linecolor);
		}
		backgroundnode->drawLine(Point(temp, totalline * j + m_Border), Point(temp, _contentSize.height - m_Border), linecolor);
	}
	//backgroundnode->drawSolidRect(Point(0, 0), Point(_contentSize.width, m_Border), bordercolor);
	//backgroundnode->drawSolidRect(Point(0, _contentSize.height), Point(_contentSize.width, _contentSize.height - m_Border), bordercolor);
	//backgroundnode->drawSolidRect(Point(0, 0), Point(m_Border, _contentSize.height), bordercolor);
	//backgroundnode->drawSolidRect(Point(_contentSize.width, 0), Point(_contentSize.width - m_Border, _contentSize.height), bordercolor);
}
void ChessBoard::start()
{
	srand(time(0) + rand());
	int i, j;
	for (i = 0; i < m_rowCount; i++)
	{
		for (j = 0; j < m_columnCount; j++)
		{
			addBlockWithDrop(newBlock(), j, i);
		}
	}
	scheduleUpdate();
}
Block* ChessBoard::newBlock(int type)
{
	if (type == -1)
	{
		return Block::create(BlockType(random(0, BlockType::Count - 1)));
	}
	else
	{
		return Block::create(BlockType(type));
	}
}
void ChessBoard::addBlockWithDrop(Block* target, int x, int y)
{
	target->x = x;
	target->y = y;
	this->addChild(target,8);
	auto size = target->getContentSize();
	target->setScaleX(m_blockSize.width / size.width);
	target->setScaleY(m_blockSize.height / size.height);
	Point startposition = this->pointToClient(x, y + m_rowCount / 2);
	Point endposition = this->pointToClient(x, y);
	target->setPosition(startposition);
	target->animeCount++;
	Blocks[y*m_columnCount + x] = target;
	target->runAction(Sequence::create(EaseSineIn::create((MoveTo::create(0.3, endposition))), CallFunc::create([target](){ target->animeCount--; }), NULL));
}
void ChessBoard::setContentSize(const Size& size)
{
	Sprite::setContentSize(size);
	if (m_columnCount <= 0 || m_rowCount <= 0)
		return;
	float blockwidth = (size.width * 100) / (m_columnCount * 100 + 2 * m_BorderP + m_MarginP*(m_columnCount - 1));
	m_Border = (int)(blockwidth*m_BorderP / 100);
	m_Margin = (int)(blockwidth*m_MarginP / 100);
	m_blockSize.width = (size.width - (m_columnCount - 1)*m_Margin - 2 * m_Border) / m_columnCount;
	m_blockSize.height = (size.height - (m_rowCount - 1)*m_Margin - 2 * m_Border) / m_rowCount;
}
void ChessBoard::setBlockSize(const Size& size)
{
	m_Border = (int)(size.width*m_BorderP / 100);
	m_Margin = (int)(size.width*m_MarginP / 100);
	float width = size.width*m_columnCount + 2 * m_Border + m_Margin*(m_columnCount - 1);
	float height = size.height*m_rowCount + 2 * m_Border + m_Margin*(m_rowCount - 1);
	Sprite::setContentSize(Size(width, height));
}
Point ChessBoard::pointToClient(const float& x, const float& y) const
{
	auto location = this->getPosition();
	float xx = m_Border + (m_Margin + m_blockSize.width)*x + m_blockSize.width / 2;
	float yy = m_Border + m_Margin + (m_Margin + m_blockSize.height)*y + m_blockSize.height / 2;
	return Point(xx, yy);
}
bool ChessBoard::autoCheck()
{
	bool result = true;
	int i;
	std::vector<Block*> list;
	list.reserve(m_totalBlockCount);
	for (i = 0; i < m_totalBlockCount; i++)
	{
		if (Blocks[i] == NULL || Blocks[i]->animeCount != 0)
		{
			if (result)
				result = false;
			continue;
		}
		Blocks[i]->chaincount = 0;
		Blocks[i]->rowChecked = false;
		Blocks[i]->colChecked = false;
	}
	for (i = 0; i < m_totalBlockCount; i++)
	{
		if (Blocks[i] == NULL || Blocks[i]->animeCount != 0)
		{
			if (result)
				result = false;
			continue;
		}
		if (check(Blocks[i], list))
		{
			if (result)
				result = false;
			clearList(list);
		}
	}
	return result;
}
bool ChessBoard::check(Block* target, std::vector<Block*>& list)
{
	list.clear();
	list.push_back(target);
	checkCol(target, list);
	checkRow(target, list);
	if (list.size() > 1)
		return true;
	return false;
}
void ChessBoard::clearList(std::vector<Block*>& list)
{
	int listcount = list.size();
	if (listcount < 1)
		return;
	int i, j, maxcount = 0, totalcount = 0;
	BlockClearEventArg eventarg;
	Block* maxblock, *temp;
	for (i = 0; i < listcount; i++)
	{
		if (list[i]->chaincount>maxcount)
		{
			maxcount = list[i]->chaincount;
			maxblock = list[i];
		}
	}
	if (maxcount + 1 > 3)
	{
		Point position = maxblock->getPosition();
		for (i = 0; i < listcount; i++)
		{
			j = pointToIndex(list[i]->x, list[i]->y);
			if (maxblock != list[i] && Blocks[j] != NULL)
			{
				temp = Blocks[j];
				Blocks[j] = NULL;
				maxblock->animeCount++;
				totalcount += list[i]->getMode();
				eventarg.blocks.push_back(list[i]);
				list[i]->runAction(Sequence::create((MoveTo::create(0.2, position)), CallFunc::create([temp, maxblock]{
					maxblock->animeCount--;
					temp->removeFromParent();
				}), NULL));
			}
		}
		if (maxblock->isLocked())
			maxblock->unlock();
		maxblock->setMode(maxcount + 1);
		eventarg.blocks.push_back(maxblock);
	}
	else
	{
		for (i = 0; i < listcount; i++)
		{
			j = pointToIndex(list[i]->x, list[i]->y);
			if (Blocks[j] != NULL)
			{
				temp = Blocks[j];
				Blocks[j] = NULL;
				totalcount += list[i]->getMode();
				eventarg.blocks.push_back(list[i]);
				list[i]->runAction(Sequence::create(ScaleTo::create(0.2, 0, 0), CallFunc::create([temp]{
					temp->removeFromParent();
				}), NULL));
			}
		}
	}
	if (m_roadChecked)
		m_roadChecked = false;
	if (m_help)
		cancelhelp();
	blockClearEvent.func(eventarg);
}
void ChessBoard::checkCol(Block* target, std::vector<Block*>& list)
{
	int x = target->x;
	int y = target->y;
	int targetIndex = pointToIndex(x, y);
	if (target->colChecked)
		return;
	int i, j, count = 0;
	int listcount = list.size();
	BlockType targettype = target->type;
	target->colChecked = true;
	for (i = y + 1; i < m_rowCount; i++)
	{
		j = pointToIndex(x, i);
		if (Blocks[j] == NULL || Blocks[j]->animeCount != 0 || Blocks[j]->colChecked || Blocks[j]->type != targettype)
			break;
		list.push_back(Blocks[j]);
		Blocks[j]->colChecked = true;
		count++;
	}
	for (i = y - 1; i >= 0; i--)
	{
		j = pointToIndex(x, i);
		if (Blocks[j] == NULL || Blocks[j]->animeCount != 0 || Blocks[j]->colChecked || Blocks[j]->type != targettype)
			break;
		list.push_back(Blocks[j]);
		Blocks[j]->colChecked = true;
		count++;
	}
	if (count < 2)
	{
		while (count>0)
		{
			list.pop_back();
			count--;
		}
	}
	else
	{
		target->chaincount += count;
		for (i = 0; i<count; i++)
		{
			list[listcount + i]->chaincount += count;
			checkRow(list[listcount + i], list);
		}
	}
}
void ChessBoard::checkRow(Block* target, std::vector<Block*>& list)
{
	int x = target->x;
	int y = target->y;
	int targetIndex = pointToIndex(x, y);
	if (target->rowChecked)
		return;
	int i, j, count = 0;
	int listcount = list.size();
	BlockType targettype = target->type;
	target->rowChecked = true;
	for (i = x + 1; i < m_columnCount; i++)
	{
		j = pointToIndex(i, y);
		if (Blocks[j] == NULL || Blocks[j]->animeCount != 0 || Blocks[j]->rowChecked || Blocks[j]->type != targettype)
			break;
		list.push_back(Blocks[j]);
		Blocks[j]->rowChecked = true;
		count++;
	}
	for (i = x - 1; i >= 0; i--)
	{
		j = pointToIndex(i, y);
		if (Blocks[j] == NULL || Blocks[j]->animeCount != 0 || Blocks[j]->rowChecked || Blocks[j]->type != targettype)
			break;
		list.push_back(Blocks[j]);
		Blocks[j]->rowChecked = true;
		count++;
	}
	if (count < 2)
	{
		while (count-->0)
		{
			list.pop_back();
		}
	}
	else
	{
		target->chaincount += count;
		for (i = 0; i<count; i++)
		{
			list[listcount + i]->chaincount += count;
			checkCol(list[listcount + i], list);
		}
	}
}
void ChessBoard::update(float dt)
{
	fillBoard();
	if (autoCheck() && !m_roadChecked&&!hasRoad())
	{
		destroyAll(0.1);
		m_roadChecked = false;
	}
}
void ChessBoard::fillBoard()
{
	int i, j, index, start;
	Block* temp;
	for (i = 0; i < m_columnCount; i++)
	{
		j = 0;
		start = -1;
		while (j < m_rowCount)
		{
			if (Blocks[index = j*m_columnCount + i] != NULL && Blocks[index]->animeCount != 0)
				break;
			if (start == -1 && Blocks[index] == NULL)
				start = j;
			j++;
		}
		if (j < m_rowCount || start == -1)
			continue;
		for (j = start + 1; j < m_rowCount; j++)
		{
			index = j*m_columnCount + i;
			if (Blocks[index] != NULL)
			{
				temp = Blocks[index];
				Blocks[index] = NULL;
				Blocks[start*m_columnCount + i] = temp;
				temp->y = start;
				temp->animeCount++;
				temp->runAction(Sequence::create((EaseSineIn::create(MoveTo::create(0.2, pointToClient(i, start)))), CallFunc::create([temp]{temp->animeCount--; }), NULL));
				start++;
			}
		}
		while (start < m_rowCount)
		{
			addBlockWithDrop(newBlock(), i, start);
			start++;
		}
	}
}
bool ChessBoard::onTouchBegan(Point location)
{
	int x, y, beginIndex;
	//Point location = touch->getLocation() - this->getPosition();
	x = ((location.x - m_Border) / (m_blockSize.width + m_Margin));
	y = ((location.y - m_Border) / (m_blockSize.height + m_Margin));
	int endIndex = x + y*m_columnCount;
	if (x<0 || x >= m_columnCount || y<0 || y >= m_rowCount || location.x - m_Border - x*(m_blockSize.width + m_Margin) < m_Margin || location.y - m_Border - y*(m_blockSize.height + m_Margin) < m_Margin)
	{
		unselect();
	}
	else if (t_beginX != -1 && t_beginY != -1 && ((abs(x - t_beginX) == 1 && y == t_beginY) || (abs(y - t_beginY) == 1 && x == t_beginX)) && Blocks[endIndex] != NULL&&Blocks[endIndex]->animeCount == 0 && Blocks[beginIndex = t_beginX + t_beginY*m_columnCount] != NULL&&Blocks[beginIndex]->animeCount == 0 && !Blocks[beginIndex]->isLocked() && !Blocks[endIndex]->isLocked())
	{
		unselect();
		trySwap(beginIndex, endIndex);
	}
	else if (Blocks[endIndex] != NULL&&Blocks[endIndex]->animeCount == 0 && Blocks[endIndex]->canPress)
	{
		unselect();
		pressBlock(endIndex);
	}
	else
	{
		unselect();
		select(x, y);
	}
	return true;
}
void ChessBoard::onTouchMoved(Point location)
{
	int beginIndex = t_beginX + t_beginY*m_columnCount;
	if (t_beginX == -1 || t_beginY == -1 || Blocks[beginIndex] == NULL || Blocks[beginIndex]->animeCount != 0)
		return;
	int x = -1, y = -1;
	//Point location = touch->getLocation() - this->getPosition();
	double dx = location.x - t_beginPoint.x;
	double dy = location.y - t_beginPoint.y;
	if (abs(dx) > m_blockSize.width / 2 && abs(dy) > m_blockSize.height / 2)
	{
		unselect();
		return;
	}
	else if (abs(dx) < m_blockSize.width / 2 && abs(dy) < m_blockSize.height / 2)
	{
		return;
	}
	else if (dx>m_blockSize.width / 2)
	{
		x = t_beginX + 1;
		y = t_beginY;
	}
	else if (dx < -m_blockSize.width / 2)
	{
		x = t_beginX - 1;
		y = t_beginY;
	}
	else if (dy>m_blockSize.height / 2)
	{
		x = t_beginX;
		y = t_beginY + 1;
	}
	else if (dy < -m_blockSize.height / 2)
	{
		x = t_beginX;
		y = t_beginY - 1;
	}
	int endIndex = x + y*m_columnCount;
	if (x < 0 || y < 0 || x >= m_columnCount || y >= m_rowCount || Blocks[endIndex] == NULL || Blocks[endIndex]->animeCount != 0)
	{
		unselect();
		return;
	}
	if (Blocks[beginIndex]->isLocked() || Blocks[endIndex]->isLocked())
		return;
	unselect();
	trySwap(beginIndex, endIndex);
}
void ChessBoard::swapBlock(int a, int b, cocos2d::CallFunc* func)
{
	int temp;
	Block* tempb;
	temp = Blocks[a]->x;
	Blocks[a]->x = Blocks[b]->x;
	Blocks[b]->x = temp;
	temp = Blocks[a]->y;
	Blocks[a]->y = Blocks[b]->y;
	Blocks[b]->y = temp;
	tempb = Blocks[a];
	Blocks[a] = Blocks[b];
	Blocks[b] = tempb;
	Blocks[a]->animeCount++;
	Blocks[b]->animeCount++;
	auto action = Spawn::create(TargetedAction::create(Blocks[a],MoveTo::create(0.2, pointToClient(Blocks[a]->x, Blocks[a]->y))), MoveTo::create(0.2, pointToClient(Blocks[b]->x, Blocks[b]->y)), NULL);
	Blocks[b]->runAction(Sequence::create(action, CallFunc::create([a, b, this]
	{
		Blocks[a]->animeCount--;
		Blocks[b]->animeCount--;
	}), func, NULL));
}
void ChessBoard::afterSwap(int index_1, int index_2)
{
	int i;
	bool success = false;
	std::vector<Block*> list;
	list.reserve(m_totalBlockCount);
	for (i = 0; i < m_totalBlockCount; i++)
	{
		if (Blocks[i] == NULL || Blocks[i]->animeCount != 0)
			continue;
		Blocks[i]->chaincount = 0;
		Blocks[i]->rowChecked = false;
		Blocks[i]->colChecked = false;
	}
	if (Blocks[index_1] != NULL && Blocks[index_1]->animeCount == 0 && check(Blocks[index_1], list))
	{
		success = true;
		clearList(list);
	}
	if (Blocks[index_2] != NULL && Blocks[index_2]->animeCount == 0 && check(Blocks[index_2], list))
	{
		success = true;
		clearList(list);
	}
	if (!success)
		swapBlock(index_1, index_2);
}
bool ChessBoard::checkSwap(int index_1, int index_2)
{
	int temp;
	Block* tempb;
	temp = Blocks[index_1]->x;
	Blocks[index_1]->x = Blocks[index_2]->x;
	Blocks[index_2]->x = temp;
	temp = Blocks[index_1]->y;
	Blocks[index_1]->y = Blocks[index_2]->y;
	Blocks[index_2]->y = temp;
	tempb = Blocks[index_1];
	Blocks[index_1] = Blocks[index_2];
	Blocks[index_2] = tempb;
	int i;
	bool success = false;
	std::vector<Block*> list;
	list.reserve(m_totalBlockCount);
	for (i = 0; i < m_totalBlockCount; i++)
	{
		if (Blocks[i] == NULL || Blocks[i]->animeCount != 0)
			continue;
		Blocks[i]->chaincount = 0;
		Blocks[i]->rowChecked = false;
		Blocks[i]->colChecked = false;
	}
	if (check(Blocks[index_1], list) || check(Blocks[index_2], list))
		success = true;
	temp = Blocks[index_1]->x;
	Blocks[index_1]->x = Blocks[index_2]->x;
	Blocks[index_2]->x = temp;
	temp = Blocks[index_1]->y;
	Blocks[index_1]->y = Blocks[index_2]->y;
	Blocks[index_2]->y = temp;
	tempb = Blocks[index_1];
	Blocks[index_1] = Blocks[index_2];
	Blocks[index_2] = tempb;
	return success;
}
bool ChessBoard::hasRoad()
{
	int x, y, index_1, index_2;
	m_roadChecked = true;
	for (y = 0; y < m_rowCount - 1; y++)
	{
		index_1 = pointToIndex(0, y);
		for (x = 0; x < m_columnCount - 1; x++, index_1++)
		{
			if (checkSwap(index_1, index_2 = index_1 + 1) || checkSwap(index_1, index_2 = index_1 + m_columnCount))
			{
				if (m_help&&helpSprite==NULL)
					help(index_1, index_2);
				return true;
			}
		}
		if (checkSwap(index_1, index_2 = index_1 + m_columnCount))
		{
			if (m_help&&helpSprite == NULL)
				help(index_1, index_2);
			return true;
		}
	}
	index_1 = pointToIndex(0, y);
	for (x = 0; x < m_columnCount - 1; x++, index_1++)
	{
		if (checkSwap(index_1, index_2 = index_1 + 1))
		{
			if (m_help&&helpSprite == NULL)
				help(index_1, index_2);
			return true;
		}
	}
	return false;
}
void ChessBoard::destroy(int i, float animetime)
{
	auto explosion = ParticleSystemQuad::create();
	explosion->setEmitterMode(ParticleSystem::Mode::GRAVITY);
	explosion->setPositionType(ParticleSystem::PositionType::FREE);
	float life;
	explosion->setSpeed(200);
	explosion->setSpeedVar(50);
	explosion->setLife(life = animetime);
	explosion->setLifeVar(0.3*life);
	explosion->setAngle(90);
	explosion->setAngleVar(360);
	explosion->setStartSize(25);
	explosion->setStartSizeVar(10);
	explosion->setEndSize(-1);
	explosion->setEndSizeVar(0);
	explosion->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	explosion->setEmissionRate(50);
	explosion->setTotalParticles(15);
	explosion->setDuration(animetime);
	explosion->setAutoRemoveOnFinish(true);
	explosion->setTexture(explosionnode->getTexture());
	explosion->setStartColor(Color4F(Color4B(255, 255, 255, 255)));
	explosion->setStartColorVar(Color4F(Color4B(0, 0, 0, 0)));
	explosion->setEndColor(Color4F(Color4B(255, 255, 255, 0)));
	explosion->setEndColorVar(Color4F(Color4B(0, 0, 0, 0)));
	explosionnode->addChild(explosion);
	auto target = Blocks[i];
	Blocks[i] = NULL;
	auto size = target->getContentSize();
	auto position = target->getPosition();
	explosion->setPosition(position.x, position.y);
	target->runAction(Sequence::create(ScaleTo::create(0.3, 0, 0), CallFunc::create([target]{
		target->removeFromParent();
	}), NULL));
}
void ChessBoard::destroyAll(float animetime)
{
	for (int i = 0; i < m_totalBlockCount; i++)
	{
		if (Blocks[i] != NULL&&Blocks[i]->animeCount == 0)
		{
			destroy(i, animetime);
		}
	}
}

void ChessBoard::select(int x, int y)
{
	t_beginX = x;
	t_beginY = y;
	t_beginPoint = pointToClient(x, y);
	selectSprite = Sprite::createWithSpriteFrameName("select.png");
	addChild(selectSprite, 10);
	auto size = selectSprite->getContentSize();
	selectSprite->setScaleX(m_blockSize.width / size.width);
	selectSprite->setScaleY(m_blockSize.height / size.height);
	selectSprite->setPosition(pointToClient(x, y));
	FiniteTimeAction* temp;
	selectSprite->runAction(RepeatForever::create(Sequence::create(temp = ScaleBy::create(0.7, 1.1), temp->reverse(), NULL)));
}
void ChessBoard::unselect()
{
	t_beginX = -1;
	t_beginY = -1;
	if (selectSprite != NULL)
	{
		selectSprite->removeFromParent();
		selectSprite = NULL;
	}
}
void ChessBoard::help(int index_1, int index_2)
{
	if (!isScheduled("help"))
	{
		scheduleOnce(std::bind(&ChessBoard::schdulehelp, this, index_1, index_2, std::placeholders::_1), m_helpTime, "help");
	}
}
void ChessBoard::schdulehelp(int index_1, int index_2, float dt)
{
	Point p1 = indexToPoint(index_1);
	Point p2 = indexToPoint(index_2);
	helpSprite = Sprite::createWithSpriteFrameName("change.png");
	addChild(helpSprite, 9);
	auto size = helpSprite->getContentSize();
	helpSprite->setScale((m_blockSize.width<m_blockSize.height?m_blockSize.width:m_blockSize.height)/ size.width*0.6);
	helpSprite->setPosition(pointToClient(((float)p1.x + p2.x) / 2, ((float)p1.y + p2.y) / 2));
	helpSprite->runAction(RepeatForever::create(RotateBy::create(6, 360)));
}
void ChessBoard::cancelhelp()
{
	if (isScheduled("help"))
		this->unschedule("help");
	else if (helpSprite != NULL)
	{
		helpSprite->removeFromParent();
		helpSprite = NULL;
	}
}
int ChessBoard::checkSwap(int index_1, int index_2, int* cfs)
{
	if (Blocks[index_1] == NULL || Blocks[index_2] == NULL || Blocks[index_1]->animeCount != 0 || Blocks[index_2]->animeCount != 0)
		return -1;
	Block* tempb;
	int temp,score = 0;
	bool result = false;
	temp = Blocks[index_1]->x;
	Blocks[index_1]->x = Blocks[index_2]->x;
	Blocks[index_2]->x = temp;
	temp = Blocks[index_1]->y;
	Blocks[index_1]->y = Blocks[index_2]->y;
	Blocks[index_2]->y = temp;
	tempb = Blocks[index_1];
	Blocks[index_1] = Blocks[index_2];
	Blocks[index_2] = tempb;
	int i;
	bool success = false;
	std::vector<Block*> list;
	list.reserve(m_totalBlockCount);
	for (i = 0; i < m_totalBlockCount; i++)
	{
		if (Blocks[i] == NULL || Blocks[i]->animeCount != 0)
			continue;
		Blocks[i]->chaincount = 0;
		Blocks[i]->rowChecked = false;
		Blocks[i]->colChecked = false;
	}
	if (check(Blocks[index_1], list))
	{
		if (!result)
			result = true;
		score += list.size()*cfs[list[0]->type];
	}
	if (check(Blocks[index_2], list))
	{
		if (!result)
			result = true;
		score += list.size()*cfs[list[0]->type];
	}
	temp = Blocks[index_1]->x;
	Blocks[index_1]->x = Blocks[index_2]->x;
	Blocks[index_2]->x = temp;
	temp = Blocks[index_1]->y;
	Blocks[index_1]->y = Blocks[index_2]->y;
	Blocks[index_2]->y = temp;
	tempb = Blocks[index_1];
	Blocks[index_1] = Blocks[index_2];
	Blocks[index_2] = tempb;
	return result?score:-1;
}

void ChessBoard::pressBlock(int index)
{
	Block* temp = Blocks[index];
	switch (temp->type)
	{
	case BlockType::Key:
		Blocks[index] = NULL;
		for (int i = 0; i < m_totalBlockCount; i++)
		{
			if (Blocks[i] != NULL&&Blocks[i]->isLocked())
				Blocks[i]->unlock();
		}
		temp->runAction(Sequence::create(ScaleTo::create(0.2, 0, 0), CallFunc::create([temp]{
			temp->removeFromParent();
		}), NULL));
		if (m_roadChecked)
			m_roadChecked = false;
		if (m_help)
			cancelhelp();
		break;
	default:
		break;
	}
	
}

void ChessBoard::allowHelp(float time)
{
	m_help = true;
	m_helpTime = time;
}
void ChessBoard::banHelp()
{
	m_help = false;
	cancelhelp();
}
void ChessBoard::lockBlock(int index,float time)
{
	Blocks[index]->lock(time);
}
bool ChessBoard::isLocked(int index)
{
	return Blocks[index] != NULL&&Blocks[index]->isLocked();
}
bool ChessBoard::isNULL(int index)
{
	return Blocks[index]==NULL;
}
int ChessBoard::getBlockType(int index)
{
	return Blocks[index]->type;
}