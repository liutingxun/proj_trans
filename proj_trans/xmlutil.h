#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <iostream>
#include "Markup.h"
#include <string>
#include <vector>
#include <map>
#include <locale>
#include "tree.h"
#include "tree_util.h"
using namespace  std;

class xmlutil
{
public:
	xmlutil(){};
	void readxml(const char *filename);
	void parsexml(const char *filepath);
	void parseFilter(const char *filepath);

	bool generatexml(const char *filepath);
	void convertPreProcessor(string &str);
	void convertAdditionalDepend(string &str);

	vector<tree<string>> stringstotrees(vector<string> strvec);
	tree<string> tokenmerge(vector<tree<string>> vec);
	void print_tree(const tree<std::string>& tr, tree<std::string>::pre_order_iterator it, tree<std::string>::pre_order_iterator end);
	tree<string> stringtotree(string str);
	void treetoxml(CMarkup& xml, const tree<std::string>& tr, tree<std::string>::pre_order_iterator it, tree<std::string>::pre_order_iterator end);
	void makefilters(CMarkup& xml, const tree<std::string>& tr, tree<std::string>::pre_order_iterator it, tree<std::string>::pre_order_iterator end);

	void putfiles(CMarkup& xml, map<string, vector<string>> files);
	string getTargetFilePath(const char* filepath);

private:
	vector<string> sourcevec_;
	vector<string> headervec_;
	string projName_;
	string projGuid_;
	string PreprocessorDefinitions_;
	string AdditionalIncludeDirectories_;
	string AdditionalDependencies_;

	string PreprocessorDefinitions_release_;

	tree<string> filter_;


	vector<string> filevec_ ;
	map<string, vector<string>> allfiles_;

	tree<string> filter_tree_;
	vector<string> filter_vec_;
	string configuretype_;
};
#endif