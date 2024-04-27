#include "pch.h"
#include "Animation.h"
#include "Texture.h"

Animation::Animation() : Super(ResourceType::Animation)
{

}

Animation::~Animation()
{
}

void Animation::Load(const wstring& path)
{
	tinyxml2::XMLDocument doc;
	
	string pathStr(path.begin(), path.end());
	XMLError error = doc.LoadFile(pathStr.c_str());
	assert(error == XMLError::XML_SUCCESS);

	XMLElement* root = doc.FirstChildElement();
	string name = root->Attribute("Name");
	_name = wstring(name.begin(), name.end());

	_loop = root->BoolAttribute("Loop");
	_path = path;

	// Load Texture

	XmlElement* node = root->FirstChildElement();

	for(; node != nullptr; node = node->NextSiblingElement())
	{
		Keyframe keyframe;

		keyframe.offset.x = stof(node->Attribute("offsetX"));
		keyframe.offset.y = stof(node->Attribute("offsetY"));
		keyframe.size.x = stof(node->Attribute("SizeX"));
		keyframe.size.y = stof(node->Attribute("SizeY"));
		keyframe.time = stof(node->Attribute("time"));

		AddKeyframe(keyframe);
	}

}

void Animation::Save(const wstring& path)
{
	tinyxml2::XMLDocument doc;
	XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	string nameStr(GetName().begin(), GetName().end());

	root->SetAttribute("Name", nameStr.c_str());
	root->SetAttribute("Loop", _loop);
	root->SetAttribute("TexturePath", "TODO");

	for (const auto& keyframe: _keyframes) 
	{
		XMLElement* frame = doc.NewElement("Keyframe");
		root->LinkEndChild(frame);

		frame->SetAttribute("offsetX", keyframe.offset.x);
		frame->SetAttribute("offsetY", keyframe.offset.y);
		frame->SetAttribute("SizeX", keyframe.size.x);
		frame->SetAttribute("SizeY", keyframe.size.y);
		frame->SetAttribute("time", keyframe.time);
	}

	string pathStr(path.begin(), path.end());
	auto result = doc.SaveFile(pathStr.c_str());

	assert(result == XMLError::XML_SUCCESS);
}

Vec2 Animation::GetTextureSize()
{
	return _texture->GetSize();
}

const Keyframe& Animation::Getkeyframe(uint32 index)
{
	return _keyframes[index];
}

int32 Animation::GetKeyframeCount()
{
	return _keyframes.size();
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	_keyframes.push_back(keyframe);
}

