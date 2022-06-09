#ifndef ONESERVER_HPP
#define ONESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include "../util/numbers.hpp"
#include "../config/Node.hpp"
#include "AServerItem.hpp"
#include "ALocation.hpp"
#include "OneLocation.hpp"

class OneLocation;
class OneServer : public ALocation
{
public:
	OneServer();
	// OneServer(const OneServer & src);
	// OneServer & operator=(const OneServer & rhs);
	~OneServer();

	AServerItem *consume(Node *node);
	virtual std::ostream &print(std::ostream &o) const;
	static DIRECTIVES_MAP initializeDirectivesMap();
	AServerItem *findRightConfig(std::list<std::string> route_target);

	t_listen getListen();

private:
	virtual AServerItem *addServerName(Node *node);
	virtual AServerItem *addLocation(Node *node);
	virtual AServerItem *addListen(Node *node);

	std::map<std::string, OneLocation *> _location;
	std::vector<std::string> _server_name;
	t_listen _listen;

protected:
	virtual AServerItem *addIndex(Node *node);
	virtual AServerItem *addRoot(Node *node);
	virtual AServerItem *addAutoIndex(Node *node);
	virtual AServerItem *addMethod(Node *node);
	virtual AServerItem *addMaxSize(Node *node);
	virtual AServerItem *addErrorPage(Node *node);
	virtual AServerItem *addCgi(Node *node);

	static DIRECTIVES_MAP _directives_to_setter;
	virtual DIRECTIVES_MAP &getDirectiveMap();
};

#endif /*------------------- OneServer --------*/