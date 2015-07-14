
#include "stdafx.h"
#include "xmlutil.h"

void xmlutil::parsexml(const char *filepath)
{

	CMarkup xml;
	bool bload = false;
	string tmp;
	bload = xml.Load(filepath);
	if(!bload)
	{
		MessageBox(NULL,"未找到相应工程文件","错误",MB_OK);
		return;
	}
	
		//xml.ResetMainPos();
		xml.FindElem("Project");//project
		xml.IntoElem();
		while (xml.FindElem("ItemGroup"))
		{
			while (xml.FindChildElem("ClCompile"))
			{
				tmp = xml.GetChildAttrib("Include");
				sourcevec_.push_back(tmp);
			}
			while(xml.FindChildElem("ClInclude"))
			{
				tmp = xml.GetChildAttrib("Include");
				headervec_.push_back(tmp);
			}
		}
		while (xml.FindElem("PropertyGroup"))
		{
			while (xml.FindChildElem("ProjectGuid"))
			{
				projGuid_ = xml.GetChildData();
			}
			while (xml.FindChildElem("RootNamespace"))
			{
				projName_ = xml.GetChildData();
			}

			while (xml.FindChildElem("ConfigurationType"))
			{
				string tmp = xml.GetChildData();
				if (tmp == "Application")
				{
					configuretype_ = "1";
				}
				else if (tmp == "DynamicLibrary")
				{
					configuretype_ = "2";
				}
				else if(tmp == "StaticLibrary")
				{
					configuretype_ = "4";
				}
			}
		}
		while (xml.FindElem("ItemDefinitionGroup"))
		{
			string cond;
			xml.GetAttribString("Condition",cond);
			string::size_type idx = cond.find("Debug");
			if(idx != string::npos)
			{
				//Debug
				xml.IntoElem();
				while(xml.FindElem("ClCompile"))
				{
					while(xml.FindChildElem("PreprocessorDefinitions"))
					{
						PreprocessorDefinitions_ = xml.GetChildData();
						convertPreProcessor(PreprocessorDefinitions_);
					}
					while(xml.FindChildElem("AdditionalIncludeDirectories"))
					{
						AdditionalIncludeDirectories_ = xml.GetChildData();
					}
				}
				while(xml.FindElem("Link"))
				{
					while(xml.FindChildElem("AdditionalDependencies"))
					{
						AdditionalDependencies_ = xml.GetChildData();
						convertAdditionalDepend(AdditionalDependencies_);
					}
				}
				xml.OutOfElem();
			}
			else
			{
				//release
				xml.IntoElem();
				while (xml.FindElem("ClCompile"))
				{
					while (xml.FindChildElem("PreprocessorDefinitions"))
					{
						PreprocessorDefinitions_release_ = xml.GetChildData();
					}
				}
				while(xml.FindElem("Link"))
				{
					;
				}
				xml.OutOfElem();

			}

		}
	
}

void xmlutil::parseFilter(const char *filepath)
{
	CMarkup xml;
	bool bload;
	string filtertmp = "";
	string filterpath = filepath;

	filterpath += string(".filters");
	
	//bload = xml.Load("./Node2.vcxproj.filters");
	bload = xml.Load(filterpath);
	filter_tree_.set_head("root");

	if(!bload)
	{
		MessageBox(NULL,"未能找到相应的filter文件","错误",MB_OK);
		exit(0);
	}
	
		while(xml.FindElem("Project"))
		{
			xml.IntoElem();
			while (xml.FindElem("ItemGroup"))
			{
				while (xml.FindChildElem("Filter"))
				{
					filtertmp = xml.GetChildAttrib("Include");
					//filtertmp = xml.GetAttrib("Include");
					allfiles_.insert(make_pair(filtertmp,filevec_));
					//在这里解析一次，把filter的路径放到一个tree中。
					filter_vec_.push_back(filtertmp);

				}
				//while (xml.FindChildElem("ClCompile"))
				//{
					xml.IntoElem();
					while(xml.FindElem("ClCompile"))
					{
						string filepath = xml.GetAttrib("Include");
						if (xml.FindChildElem("Filter"))
						{
							string filter = xml.GetChildData();
							map<string, vector<string>>::iterator it;
							for (it = allfiles_.begin(); it != allfiles_.end(); ++it)
							{
								if(strcmp(filter.c_str(), it->first.c_str()) == 0)
								{
									it->second.push_back(filepath);
								}
							}
						}
					}
					xml.OutOfElem();
				//}
			//	while (xml.FindChildElem("ClInclude"))
			//	{
					xml.IntoElem();
					while (xml.FindElem("ClInclude"))
					{
						string filepath = xml.GetAttrib("Include");
						if (xml.FindChildElem("Filter"))
						{
							string filter = xml.GetChildData();
							map<string, vector<string>>::iterator it;
							for (it = allfiles_.begin(); it != allfiles_.end(); ++it)
							{
								if (strcmp(filter.c_str(), it->first.c_str()) == 0)
								{
									it->second.push_back(filepath);
								}
							}
						}
					}
					xml.OutOfElem();
			//	}
			}
			xml.OutOfElem();
		}


		//将filter_vec处理一下，生成树结构。
		vector<tree<string>> vts = stringstotrees(filter_vec_);
		filter_ = tokenmerge(vts);
		//print_tree(filter_, filter_.begin(), filter_.end());
		//test,遍历输出一下。
		/*map<string, vector<string>>::iterator it;
		vector<string>::iterator itvec;
		for (it = allfiles.begin(); it != allfiles.end(); ++it)
		{
		cout << it->first<<endl;
		for (itvec = it->second.begin(); itvec != it->second.end(); ++itvec)
		{

		cout << *itvec<<endl;
		}
		}*/
	
}

tree<string> xmlutil::tokenmerge(vector<tree<string>> vec)
{
	tree<string> ts;
	ts.set_head("src");
	if (vec.size() < 2)
	{
		ts = vec.back();
	}
	else
	{
		for (vector<tree<string>>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			tree<string> tmp = *it;
			ts.merge(ts.begin(), ts.end(), tmp.begin(), tmp.end(), true);
		}
	}
	return ts;
}

vector<tree<string>> xmlutil::stringstotrees(vector<string> strvec)
{
	vector<tree<string>> vec;
	for (vector<string>::iterator it = strvec.begin(); it != strvec.end(); it++)
	{
		string str = *it;
		tree<string> strtree = stringtotree(str);
		vec.push_back(strtree);
	}
	return vec;
}

tree<string> xmlutil::stringtotree(string str)
{
	tree<string> ts;
	tree<string>::pre_order_iterator it;
	it = ts.set_head("src");
	char*  tok = strtok(const_cast<char*>(str.c_str()), "\\");
	while(tok != NULL)
	{
		it = ts.append_child(it, tok);
		tok = strtok(NULL, "\\");
	}
	return ts;
}

void xmlutil::treetoxml(CMarkup& xml, const tree<std::string>& tr, tree<std::string>::pre_order_iterator it, tree<std::string>::pre_order_iterator end)
{
	if(!tr.is_valid(it)) return;
	int rootdepth = tr.depth(it);
	while(it!= end)
	{
		for (int i = 0; i < tr.depth(it) - rootdepth; ++i)
		{
			xml.IntoElem();
		}
		xml.AddElem("Filter");
		xml.AddAttrib("Name",*it);
		for(int i = 0; i< tr.depth(it)-rootdepth; ++i)
		{
			xml.OutOfElem();
		}

		++it;
	}
}


void xmlutil::makefilters(CMarkup& xml, const tree<std::string>& tr, tree<std::string>::pre_order_iterator it, tree<std::string>::pre_order_iterator end)
{
	while(it != end )
	{
		if((++it) == end)
		{
			--it;
			xml.AddElem("Filter");
			xml.AddAttrib("Name",*it);
			return;
		}
		--it;
		xml.AddElem("Filter");
		xml.AddAttrib("Name",*it);
		if (tr.depth(++it) > tr.depth(--it))
		{
			for(int i = 0; i< tr.depth(++it) - tr.depth(--it); i++)
			{
				xml.IntoElem();
			}
		}
		else if(tr.depth(++it) < tr.depth(--it))
		{
			for (int i = 0; i <tr.depth(it) - tr.depth(++it);i++)
			{
				--it;
				xml.OutOfElem();
			}
			--it;
		}

		it++;
	}


}


void xmlutil::putfiles(CMarkup& xml, map<string, vector<string>> files)
{
	while(xml.FindElem("Filter") )
	{
		if (xml.GetAttrib("Name") == "src")
		{

			xml.IntoElem();
			xml.SavePos("source");
			for (map<string, vector<string>>::iterator it = files.begin(); it != files.end(); it++)
			{
				string str = it->first;
				//取出路径名后，依据\进行拆分
				char* tok = strtok(const_cast<char*>(str.c_str()), "\\");
				while (tok != NULL)
				{
					//然后根据每层的路径名找寻对应的xml元素
					while (xml.FindElem("Filter"))
					{
						if(xml.GetAttrib("Name") ==tok)
						{
							xml.IntoElem();
							break;
						}

					}
					tok = strtok(NULL,"\\");
				}
				//找到对应目录后，将文件存放进去
				for (vector<string>::iterator it_vec = it->second.begin(); it_vec != it->second.end();it_vec++)
				{
					xml.AddElem("File");
					xml.AddAttrib("RelativePath",*it_vec);
				}
				xml.RestorePos("source");
			}
		}
	}
}

bool xmlutil::generatexml(const char* filepath)
{
	if (filepath == NULL)
	{
		return false;
	}
	CMarkup xml;
	xml.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\n");

	xml.AddElem("VisualStudioProject");
	xml.AddAttrib("\nProjectType", "Visual C++");
	xml.AddAttrib("\nVersion","8.00");
	xml.AddAttrib("Name",projName_);
	xml.AddAttrib("ProjectGUID",projGuid_);
	xml.IntoElem();

	//platforms
	xml.AddElem("Platforms");
	xml.IntoElem();
	xml.AddElem("Platform");
	xml.AddAttrib("Name", "Win32");
	xml.OutOfElem();

	//ToolFiles
	xml.AddElem("ToolFiles");

	//configurations
	xml.AddElem("Configurations");
	xml.IntoElem();
	//configuration_Debug
	xml.AddElem("Configuration");
	xml.AddAttrib("Name","Debug|Win32");
	xml.AddAttrib("OutputDirectory","$(SolutionDir)$(ConfigurationName)");
	xml.AddAttrib("IntermediateDirectory","$(ConfigurationName)");
	xml.AddAttrib("ConfigurationType",configuretype_);

	xml.IntoElem();
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCPreBuildEventTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCCustomBuildTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCXMLDataGeneratorTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCWebServiceProxyGeneratorTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCMIDLTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCCLCompilerTool");
	xml.AddAttrib("Optimization","0");
	xml.AddAttrib("AdditionalIncludeDirectories",AdditionalIncludeDirectories_);
	xml.AddAttrib("PreprocessorDefinitions",PreprocessorDefinitions_);
	xml.AddAttrib("DebugInformationFormat","4");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCManagedResourceCompilerTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCResourceCompilerTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCPreLinkEventTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCLinkerTool");
	xml.AddAttrib("AdditionalDependencies",AdditionalDependencies_);
	xml.AddAttrib("GenerateDebugInformation","true");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCALinkTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCManifestTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCXDCMakeTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCBscMakeTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCFxCopTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCAppVerifierTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCWebDeploymentTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCPostBuildEventTool");

	xml.OutOfElem();//end of configuration_debug

	//configuration_release
	xml.AddElem("Configuration");
	xml.AddAttrib("Name","Release|Win32");
	xml.AddAttrib("IntermediateDirectory","$(ConfigurationName)");
	xml.AddAttrib("ConfigurationType","1");

	xml.IntoElem();
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCPreBuildEventTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCCustomBuildTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCXMLDataGeneratorTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCWebServiceProxyGeneratorTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCMIDLTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCCLCompilerTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCManagedResourceCompilerTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCResourceCompilerTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCPreLinkEventTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCLinkerTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCALinkTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCManifestTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCXDCMakeTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCBscMakeTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCFxCopTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCAppVerifierTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCWebDeploymentTool");
	xml.AddElem("Tool");
	xml.AddAttrib("Name","VCPostBuildEventTool");
	xml.OutOfElem();//end of configuration_release
	xml.OutOfElem();//end of configurations

	//reference
	xml.AddElem("References");

	//files
	xml.AddElem("Files");
	xml.IntoElem();


	map<string, vector<string>>::iterator it ;

	vector<string>::iterator itvec;

	xml.SavePos("filters");
	//创建filter
	makefilters(xml, filter_, filter_.begin(), filter_.end());

	xml.RestorePos("filters");



	//创建好filter后，要把所有的文件放到对应的filter之下
	putfiles(xml, allfiles_);





	xml.OutOfElem();//end of files
	xml.OutOfElem();//end of VisualStudio Project

	xml.Save(filepath);
	return true;
}
//将.vcxproj文件名转换成vcproj文件名
string xmlutil::getTargetFilePath(const char* filepath)
{
	string tmp = filepath;
	int idx = tmp.find_last_of("x");
	tmp.erase(idx,1);
	return tmp;
}

//把最后的%(PreProcessorDefination)去掉
void xmlutil::convertPreProcessor(string &str)
{
	int idx = str.find_last_of("%");
	str.erase(idx);
}

//去掉多余的依赖库
void xmlutil::convertAdditionalDepend(string &str)
{
	int idx = str.find(";kernel");
	str.erase(idx);
}