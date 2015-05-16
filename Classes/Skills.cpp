#include "Skills.h"
#include "Player.h"
#include "ChessBoard.h"
#include "Block.h"
#include "GameLayer.h"

using namespace cocos2d;


AS_FullSalvo* AS_FullSalvo::m_instance = 0;
void AS_FullSalvo::excute(Player* skiller, Player* target,BattleLayer_2P* layer) const
{
	auto chessboard = skiller->getChessBoard();
	int i, count = chessboard->getTotalBlockCount();
	Block* tempB;
	std::vector<Block*> chainlist;
	for (i = 0; i < count; i++)
	{
		tempB = chessboard->Blocks[i];
		if (tempB != NULL&&tempB->type == BlockType::Attack)
		{
			tempB->chaincount = 0;
			chainlist.push_back(tempB);
			chessboard->Blocks[i] = NULL;
			tempB->runAction(Sequence::create(ScaleTo::create(0.2, 0, 0), CallFunc::create([tempB]{
				tempB->removeFromParent();
			}), NULL));
		}
	}
	layer->PlayerAttack(skiller, target, chainlist);
}