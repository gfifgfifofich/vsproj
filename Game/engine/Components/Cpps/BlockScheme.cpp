
#include "../Engine.h"
#include "../Include/BlockScheme.h"

void block::Process()
{

}

void BlockScheme_Compile() // compile scene, check all block connections, create and link lines.
{
	std::vector<BlockLine> possibleblocklones;
	std::vector<uint16_t> BranchBlocks;

	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].event)
		{
			BlockLine bl;
			possibleblocklones.push_back(bl);
			possibleblocklones.back().blocks.push_back(&blocks[i]);

			uint16_t CurrentBId = i;

			bool go = true;
			while (go)
			{
				if (blocks[CurrentBId].ActionConnections.size() <= 2) // this isnt a branch/loop
				{
					// finding dependencies
					// a new line, that may no exist, that is a reversed representation of a new passive line linked to this node
					std::vector<uint16_t> passivedataline;
					for (int a = 0; a < blocks[CurrentBId].Connections.size(); a++)
					{
						if (!blocks[CurrentBId].Connections[a]->source && !blocks[CurrentBId].Connections[a]->DataType == 0)
						{// in a data connection request
							if (blocks[blocks[CurrentBId].Connections[a]->BlockId].passive)
							{
								passivedataline.push_back(blocks[CurrentBId].Connections[a]->BlockId);
							}
						}
					}
					if (passivedataline.size() > 0)
					{
						int lastId = 0;// after every loop change it to passivedataline.size()
						bool firstrun = true;
						while (firstrun || lastId != passivedataline.size())
						{
							firstrun = false;
							int sizebuff = passivedataline.size();
							for (int j = lastId; j < sizebuff; j++)
								for (int a = 0; a < blocks[passivedataline[j]].Connections.size(); a++)
								{
									if (!blocks[passivedataline[j]].Connections[a]->source && !blocks[passivedataline[j]].Connections[a]->DataType == 0)
									{// in a data connection request
										if (blocks[blocks[passivedataline[j]].Connections[a]->BlockId].passive)
										{
											passivedataline.push_back(blocks[passivedataline[j]].Connections[a]->BlockId);
										}
									}
								}
							lastId = sizebuff;
						}

						BlockLine bl;
						for (int a = passivedataline.size() - 1; a >= 0; a--)
						{
							bl.blocks.push_back(&blocks[passivedataline[a]]);
						}
						blockLines.push_back(bl);
					}
					if (blocks[CurrentBId].ActionConnections.size() == 1)
						go = false;
					if (blocks[CurrentBId].ActionConnections.size() == 2)
					{
						if (blocks[CurrentBId].ActionConnections[0].rc.source)
						{
							if (blocks[CurrentBId].ActionConnections[0].rc.BlockId == 0)
								go = false;
							else CurrentBId = blocks[CurrentBId].ActionConnections[0].rc.BlockId;
						}
						if (blocks[CurrentBId].ActionConnections[1].rc.source)
						{
							if (blocks[CurrentBId].ActionConnections[0].rc.BlockId == 0)
								go = false;
							else CurrentBId = blocks[CurrentBId].ActionConnections[1].rc.BlockId;
						}
					}

				}
				else
				{
					BranchBlocks.push_back(CurrentBId);
					go = false;
				}
			}
		}

	}


}

void BlockScheme_Process() // Process all lines
{

}
