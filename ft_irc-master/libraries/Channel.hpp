#pragma once 
#include "../libraries/Server.hpp"
class Channel
{
	private:
		string _channelName;
		string _channelPass;
		vector<Client> _channelClients;
		int _limit;
	public:
		int getLimit() const; 
		void setChannelPass(string pass);
		string getName() const;
		void setName(string name);
		Channel();
		~Channel();
		
};