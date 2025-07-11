#pragma once

#include <Siv3D.hpp>

namespace xml
{
	class XMLWriter;

	class XMLNode
	{
	public:

		XMLNode(String NewTag, XMLNode* ParentNode = nullptr)
			: Tag(NewTag)
		{
			SetupParent(ParentNode);
		}

		XMLNode& AddAttribute(const String& Name, const char32* Value)
		{
			Attributes.emplace_back(Name, Value);
			return *this;
		}

		XMLNode& AddAttribute(const String& Name, const String& Value)
		{
			Attributes.emplace_back(Name, Value);
			return *this;
		}

		XMLNode& AddAttribute(const String& Name, const StringView Value)
		{
			Attributes.emplace_back(Name, Value);
			return *this;
		}

		XMLNode& AddAttribute(const String& Name, int32 Value)
		{
			Attributes.emplace_back(Name, ToString(Value));
			return *this;
		}

		XMLNode& AddAttribute(const String& Name, double Value)
		{
			Attributes.emplace_back(Name, ToString(Value));
			return *this;
		}

		XMLNode& AddAttribute(const String& Name, bool Value)
		{
			Attributes.emplace_back(Name, ToString(Value));
			return *this;
		}

	private:

		void SetupParent(XMLNode* ParentNode)
		{
			if (ParentNode)
			{
				Parent = ParentNode;
				Parent->Children.push_back(this);
			}
		}

	private:

		friend class XMLWriter;

		String Tag;

		Array<std::pair<String, String>> Attributes;

		XMLNode* Parent;

		Array<XMLNode*> Children;

	};

	class XMLWriter
	{
	public:

		class NodeProxy
		{
		public:

			NodeProxy(XMLNode& NodeRef)
				: Node(NodeRef)
			{
			}

			XMLNode* operator->()const
			{
				return &Node;
			}

			XMLNode* operator*()const
			{
				return &Node;
			}

		private:

			XMLNode& Node;

		};

		XMLWriter() = default;
		~XMLWriter()
		{

		}

	public:

		NodeProxy CreateNode(const String& Tag, XMLNode* ParentNode = nullptr)
		{
			XMLNode* newNode = new XMLNode(Tag, ParentNode);
			AllNodes.push_back(newNode);
			return NodeProxy(*newNode);
		}

		void Save(FilePath SavePath)
		{
			String xml;
			xml += U"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

			Array<XMLNode*> rootNodes = AllNodes.filter(
				[](XMLNode* Item)->bool
				{
						return Item && Item->Parent == nullptr;
				}
			);

			for (auto& node : rootNodes)
			{
				ConvertToXML(xml, node, 0);
			}

			TextWriter writer(SavePath);
			writer.write(xml);
			writer.close();
		}

		void Clear()
		{
			for (auto& node : AllNodes)
			{
				if (!node)
					continue;
				delete node;
				node = nullptr;
			}
			AllNodes.release();
		}

	private:

		void ConvertToXML(String& OutXML, XMLNode* Node, int32 Level)
		{
			if (!Node)
				return;

			String indent;
			for (int32 i = 0; i < Level; ++i)
			{
				indent += U"\t";
			}

			OutXML += U"{}<{}"_fmt(indent, Node->Tag);
			for (auto& [tag, value] : Node->Attributes)
			{
				OutXML += U" {}=\"{}\""_fmt(tag, value);
			}

			if (Node->Children.empty())
			{
				OutXML += U"/>\n";
			}
			else
			{
				OutXML += U">\n";
				for (auto& child : Node->Children)
				{
					ConvertToXML(OutXML, child, Level + 1);
				}
				OutXML += U"{}</{}>\n"_fmt(indent, Node->Tag);
			}
		}

	private:

		Array<XMLNode*> AllNodes;

	};

	void TestCode();
}
