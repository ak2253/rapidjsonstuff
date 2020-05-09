#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/schema.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>
#include <algorithm>

#define RAPIDJSON_NO_SIZETYPEDEFINE

using namespace rapidjson;

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

#include "HelperFuncts.h"

void jParseError(Document &sd, char *readBuffer)
{
	int off = sd.GetErrorOffset();
	fprintf(stderr, "\nError(offset %d): %s\n",
			off,
			GetParseError_En(sd.GetParseError()));

	int WIN = 30;
	int st = (off - WIN);
	st = st < 0 ? 0 : st;
	int en = off + WIN;
	en = en > 65536 ? 65536 : en;
	for (int j = st; j < en; j++)
		fprintf(stderr, "%c", readBuffer[j]);
	fprintf(stderr, "\n");
}
bool validate_Schema(vector<string> schemapath, string schemaValidator)
{
	FILE *sp = fopen(schemaValidator.c_str(), "r");
	char readBuffer[65536];
	FileReadStream is(sp, readBuffer, sizeof(readBuffer));

	Document sd;
	if (sd.ParseStream(is).HasParseError())
	{
		jParseError(sd, readBuffer);
		fclose(sp);
		return 1;
	}
	fclose(sp);

	cout << "Parsed " << schemaValidator << endl;

	SchemaDocument schema(sd); // Compile a Document to SchemaDocument

	cout << "Converted to schema" << endl;

	for (vector<string>::iterator it = schemapath.begin(); it != schemapath.end(); ++it)
	{
		cout << "Validating " << *it << endl;
		FILE *fp = fopen(it->c_str(), "r");
		char readBuffer2[65536];
		FileReadStream ts(fp, readBuffer2, sizeof(readBuffer2));

		Document d;
		if (d.ParseStream(ts).HasParseError())
		{
			jParseError(d, readBuffer2);
			fclose(fp);
		}
		fclose(fp);
		SchemaValidator validator(schema);
		if (!d.Accept(validator))
		{
			// Input JSON is invalid according to the schema
			// Output diagnostic information
			StringBuffer sb;
			validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
			cout << "Invalid schema " << sb.GetString() << endl;
			cout << "Invalid keyword " << validator.GetInvalidSchemaKeyword() << endl;
			sb.Clear();
			validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
			cout << "Invalid document " << sb.GetString() << endl;
			return false;
		}
	}
	return true;
}
bool checkParams(string parameters, vector<string> path)
{
	cout << "parsing parameters" << endl;
	string tempstr = parameters;
	if (tempstr.compare("") == 0)
	{
		return false;
	}
	string delimiter[] = {",", " "};
	int index, whindex, i;
	string token;
	string datatype = "";
	vector<string> params;
	while ((index = tempstr.find(delimiter[0])) != std::string::npos)
	{
		token = trim(tempstr.substr(0, index));
		while ((whindex = token.find(delimiter[1])) != std::string::npos)
		{
			if (token[whindex + delimiter[1].length()] == '=') //TODO:: find a better solution for default
				break;
			datatype += token.substr(0, whindex) + " ";
			token.erase(0, whindex + delimiter[1].length());
		}
		params.push_back(token);
		params.push_back(trim(datatype));
		datatype = "";
		tempstr.erase(0, index + delimiter[0].length());
	}
	while ((whindex = tempstr.find(delimiter[1])) != std::string::npos)
	{
		if (tempstr[whindex + delimiter[1].length()] == '=') //TODO:: find a better solution for default
			break;
		datatype += tempstr.substr(0, whindex) + " ";
		tempstr.erase(0, whindex + delimiter[1].length());
	}
	params.push_back(tempstr);
	params.push_back(trim(datatype));
	for (vector<string>::iterator it = path.begin(); it != path.end(); it++)
	{
		i = 0;
		FILE *file = fopen(it->c_str(), "r");
		cout << "checking parameters with " << *it << endl;
		if (!file)
		{
			std::cout << "Error file " << *it << " not found" << endl;
			fclose(file);
			return false;
		}
		else
		{
			//JSON begins
			char rBuffer[1024];
			FileReadStream is(file, rBuffer, sizeof(rBuffer));
			Document doc;
			doc.ParseStream(is);
			fclose(file);
			string SchemaName = doc["title"].GetString();
			auto &propertyname = doc[SchemaName.c_str()];
			for (Value::ConstMemberIterator iter = propertyname.MemberBegin(); iter != propertyname.MemberEnd(); iter++)
			{
				string var = iter->name.GetString();
				//cout<<var<<endl;
				//cout<<params.at(i)<<endl;
				if (var.compare("Encode") == 0)
				{
					bool flag = false;
					for (Value::ConstValueIterator diter = iter->value.Begin(); diter != iter->value.End(); diter++)
					{
						const Value &texttype = *diter;
						for (Value::ConstMemberIterator diter2 = texttype.MemberBegin(); diter2 != texttype.MemberEnd(); diter2++)
						{
							string data1 = diter2->name.GetString();
							string data2 = diter2->value["type"].GetString();
							if ((data1.compare(params.at(i)) == 0 && data2.compare(params.at(i + 1)) == 0))
							{
								flag = true;
								continue;
								break;
							}
						}
					}
					if (flag == false)
					{
						cout << "Expected to recieve one of two datatype for Encode but recieved " << params.at(i + 1) << " " << params.at(i) << endl;
						break;
					}
				}
				else if (iter->value.FindMember("default") != iter->value.MemberEnd())
				{
					if (i == params.size())
					{
						return true;
					}
					if (params.at(i).find("=") != std::string::npos)
					{
						string Varparam = trim(params.at(i).substr(0, params.at(i).find("=")));
						string defaultValue = trim(params.at(i).substr(params.at(i).find("=") + 1, params.at(i).size()));
						cout << Varparam << " " << defaultValue << endl;
						string defDataType = iter->value["type"].GetString();
						if (var.compare(Varparam) == 0 || defDataType.compare(params.at(i + 1)) == 0)
						{
							if (defDataType.compare("usint") == 0 || defDataType.compare("int") == 0 || defDataType.compare("float") == 0)
							{
								string::const_iterator numiter = defaultValue.begin();
								bool nondigit = false;
								bool decimal = false;
								while (numiter != defaultValue.end())
								{
									if (!isdigit(*numiter))
									{
										if (decimal == false && *numiter == '.')
										{
											decimal = true;
											++numiter;
											continue;
										}
										nondigit = true;
										break;
									}
									++numiter;
								}
								if (nondigit)
								{
									cout << "Expected variable of type " << defDataType << " but received " << defaultValue << endl;
									break;
								}
							}
						}
					}
				}
				else if (var.compare(params.at(i)) == 0)
				{
					string dtype = iter->value["type"].GetString();
					if (!dtype.compare(params.at(i + 1)) == 0)
					{
						cout << "Expected datatype " << dtype << " but received " << params.at(i + 1) << "in " << params.at(i + 1) << " " << params.at(i) << endl;
						break;
					}
				}
				else
				{
					cout << "Expected parameter " << var << " but received " << params.at(i) << "in " << params.at(i + 1) << " " << params.at(i) << endl;
					break;
				}
				i += 2;
			}
		}
		if (i == params.size())
			return true;
	}
	return false;
}
bool check(string parameters)
{
	vector<string> schemelist;
	schemelist.push_back("stst.json");
	if (validate_Schema(schemelist, "Validate.json") && checkParams(parameters, schemelist))
	{
		return true;
	}
	return false;
}
