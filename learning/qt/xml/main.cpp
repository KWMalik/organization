#include <iostream>
#include "QtXml/QtXml"

using namespace std;

int main(int argv, char ** argc)
{
	QDomDocument doc("test");
	QFile file("test.xml");
	if(!file.open(QIODevice::ReadOnly))
	{
		return 1;
	}

	if(!doc.setContent(&file))
	{
		return 2;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement();
		if(!e.isNull())
		{
			cout << qPrintable(e.tagName()) << endl;
		}
		n = n.nextSibling();
	}

	return 0;
}
