#include "XML.hpp"

void xml::TestCode()
{
	XMLWriter xml;
	auto root = xml.CreateNode(U"root");
	{
		auto characters = xml.CreateNode(U"characters", *root);
		auto mob1 = xml.CreateNode(U"character", *characters);
		mob1->AddAttribute(U"name", U"mob1")
			.AddAttribute(U"age", 20)
			.AddAttribute(U"sex", U"male");
		auto mob2 = xml.CreateNode(U"character", *characters);
		mob2->AddAttribute(U"name", U"mob2")
			.AddAttribute(U"age", 20)
			.AddAttribute(U"sex", U"female");
	}
	{
		auto skils = xml.CreateNode(U"skils", *root);
		auto fire = xml.CreateNode(U"skil", *skils);
		fire->AddAttribute(U"name", "fire")
			.AddAttribute(U"cost", 8)
			.AddAttribute(U"power", 15);
		auto ice = xml.CreateNode(U"skil", *skils);
		ice->AddAttribute(U"name", "ice")
			.AddAttribute(U"cost", 10)
			.AddAttribute(U"power", 20);
	}

	xml.Save(U"xml/test.xml");
}
