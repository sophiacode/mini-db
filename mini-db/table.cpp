#include<iostream>
#include<string>
#include<fstream>

#include "database.h"
#include "table.h"
#include "sqlstatement.h"

using namespace std;


/**
*  \brief ���캯��
*/
Table::Table(std::string new_path)
{
	path = new_path;			/* ��ȡ���洢·�� */
}

/**
*  \brief �������������������ڴ��
*/
Table::~Table()
{
	fstream file_stream_;
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file.c_str(), ios::out | ios::binary);
	file_stream_.write((char*)(&idPool), sizeof(idPool));
	file_stream_.close();
}

/**
*  \brief ���ñ���
*/
void Table::SetTableName(std::string new_name)
{
	table_name = new_name;
}

/**
*  \brief ��Ŀ������ɹ�����true��ʧ�ܷ���false
*/
bool Table::UseTable()
{
	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ��������򿪱�ͷ�ļ� */
	fstream fp_fields;
	fp_fields.open(table_name_fields.c_str(), std::ios::in);
	if (!fp_fields.is_open())										/* �����ʧ�ܣ��򷵻�false */
	{
		std::cout << table_name_fields << endl;
		return false;
	}

	char type[2], field_name[20],records_numb[4];
	fp_fields.read(records_numb, sizeof(char)* 4);				/* ��ȡ��ǰ��¼���� */
	records_num = atoi(records_numb);

	if (fields.size() == 0)										/* �������ǵ�һ�ζ���ʱ */
	{
		while (fp_fields.read(type, sizeof(char)* 2))				/* ��ȡ�ֶζ�Ӧ���������ͽ����ڴ� */
		{
			Field temp;
			if (type[0] == '0') temp.SetFieldType(kIntegerType);	/* 0-���ͣ�1-�ַ��� */
			else temp.SetFieldType(kStringType);
			fp_fields.read(field_name, sizeof(char)* 20);			/* ��ȡ�ֶ����ƽ����ڴ� */
			temp.SetFieldName(field_name);
			fields.push_back(temp);
		}
		fp_fields.close();											/* �ر��ļ� */

		fstream file_stream_;										/* ���������ڴ�� */
		std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
		file_stream_.open(pool_file, ios::out | ios::binary);
		file_stream_.read((char*)(&idPool), sizeof(idPool));
		file_stream_.close();
	}
	return true;
}

/**
*  \brief �ҵ��ֶζ�Ӧ���������
*/
int Table::FindIndex(std::string index_field_name)
{
	for (int i = 0; i < indexs.size(); i++)
	{
		if (index_field_name == indexs[i].GetFieldName())
		{
			return i;
		}
	}
	return -1;
}

/**
*  \brief ��ȡ������
*/
std::string Table::GetTableName()
{
	return table_name;
}

/**
*  \brief ��ȡ��ͷ��Ϣ
*/
std::vector<Field> Table::GetTableFields()
{
	return fields;
}

/**
*  \brief �����±�
*/
bool Table::CreateTable(SQLCreateTable &sql)
{
	table_name = sql.GetTableName();	/* ��ñ����� */
	if (table_name.size() >= 20)		/* �������ƹ�����ʧ�� */
	{
		std::cout << "�����ƹ�����" << endl;
		return false;
	}

	std::string cmd = "md " + path + "\\" + table_name;/* ������������ļ��� */
	if (Table::UseTable())							/* �жϱ��Ƿ��Ѿ����� */
	{
		std::cout << "�Ѵ��ڸñ���" << endl;
		return false;
	}
	else {
		if (!system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* ��ȡ��ͷ���� */

			fstream fp;
			std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ���table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::out);		/* ������ͷ�ļ� */

			for (int i = 0; i < fields.size(); i++)						/* �ж��Ƿ����������ֶ� */
			{
				for (int j = 0; j < i; j++)
				{
					if (fields[i].GetFieldName() == fields[j].GetFieldName())
					{
						std::cout << "���������ֶΣ�" << endl;
						return false;									/* �����������ֶΣ��򷵻�false */
					}
				}
			}

			char records_num[4];										/* ǰ4λд���¼���� */
			itoa(0, records_num, 10);
			fp.write(records_num, 4);
			
			for (int i = 0; i < fields.size(); i++)						/* д���ͷ���� */
			{
				std::string name = fields[i].GetFieldName() + '\0';
				ValueType type = fields[i].GetFieldType();				/* ��ȡ�ֶζ�Ӧ���������� */
				std::string type_;										/* type_����������� */
				if (type == kIntegerType) type_ = "0\0";				/* ���0�����Σ����1���ַ��� */
				else type_ = "1\0";
				fp.write(type_.c_str(), 2);								/* ��������ռ2λ���ֶ���20λ */
				fp.write(name.c_str(), 20);
			}
			fp.close();													/* �ر��ļ� */

			fstream file_stream_;										/* ���������洢�ص������ļ� */
			std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
			file_stream_.open(pool_file, ios::out | ios::binary);
			file_stream_.close();

			std::cout << "�����ɹ���" << endl;
			return true;
		}
		else {
			std::cout << "��������ʧ�ܣ�" << endl;
			return false;
		}
	}
}

/**
*  \brief ͨ��SQLֱ�Ӳ��Ҽ�¼
*/
bool Table::SelectRecord(SQLSelect &sql)
{
	table_name = sql.GetTableName();
	/*int field_id = Table::FindIndex(sql.GetField());
	int id = indexs.at(field_id).SearchNode(sql.GetValue().GetValueData());
	select_id.push_back(id);*/
	Table::Display();
	return false;
}

/**
*  \brief Delete����Select����
*/
bool Table::SelectRecord(SQLDelete &sd)
{
	table_name = sd.GetTableName();
	int field_id = Table::FindIndex(sd.GetField());
	return false;
}

/**
*  \brief Update����Select����
*/
bool Table::SelectRecord(SQLUpdate &su)
{
	table_name = su.GetTableName();
	int field_id = Table::FindIndex(su.GetWhereField());
	return false;
}

/**
*  \brief �����¼�¼
*/
bool Table::CreateRecord(SQLInsert &st)
{
	table_name = st.GetTableName();							/* ��ȡ������ */

	if (Table::UseTable())									/* ��Ŀ����ļ� */
	{
		if (records_num >= record_max - 1) return false;	/* �жϵ�ǰ���Ƿ����� */
		std::vector<string> records__data;					/* records__data�洢Ҫ��������� */
		for (int i = 0; i < fields.size(); i++)				/* ��ʼֵ�ÿ� */
		{
			std::string temp = "";
			records__data.push_back(temp);
		}

		/* ---------------------------------------------ƥ���ֶ���ֵ----------------------------------------------------- */
		if (st.IsInputField())
		{/* �������valueָ������Ӧ�ֶ�ʱ */
			for (int i = 0; i < st.GetValues().size(); i++)						/* �ж��ַ������� */
			{
				if (st.GetValues().at(i).GetValueData().size() >= record_len)			/* ���ַ�����������洢ʧ�� */
				{
					std::cout << "�����ֵ������" << endl;
					return false;
				}
			}

			int key;										/* key�жϲ����Ƿ�ɹ� */
			for (int i = 0; i < st.GetFields().size(); i++) /* ����ƥ���ƶ��ֶ����ͷ�ֶ� */
			{
				key = 0;
				for (int j = 0; j < fields.size(); j++)
				{
					if (st.GetFields().at(i) == fields[j].GetFieldName())	/* ƥ�䵽ͬ���ֶ�key = 1 */
					{
						key = 1;
						if (st.GetValues().at(i).GetValueType() == kNullType
							|| st.GetValues().at(i).GetValueType() == fields[j].GetFieldType())/* ����������Ҳƥ�䣬�����¼ */
						{
							records__data.at(j) = st.GetValues().at(i).GetValueData();
						}
						else {
							std::cout << "�������Ͳ�ƥ�䣡" << endl;
							return false;									/* ͬ���ֶ��������Ͳ�ƥ�䣬������¼ʧ�� */
						}
					}
				}
				if (key == 0)
				{
					std::cout << "�ֶ���ƥ��ʧ�ܣ�" << endl;
					return false;					/* ��ǰ�ֶ���δƥ�䵽ͬ����ͷ�ֶΣ�����ʧ�� */
				}
			}
		}
		else {/* �������valueδָ����Ӧ�ֶ�ʱ */
			if (st.GetValues().size() > fields.size()) return false;		/* ������value��Ŀ�����ֶ����������ʧ�� */
			else {
				for (int i = 0; i < st.GetValues().size(); i++)						/* �ж��ַ������� */
				{
					if (st.GetValues().at(i).GetValueData().size() >= record_len)			/* ���ַ�����������洢ʧ�� */
					{
						std::cout << "�����ֵ������" << endl;
						return false;
					}
				}

				for (int i = 0; i < st.GetValues().size(); i++)				/* ��Ĭ��˳�����value */
				{
					if (st.GetValues().at(i).GetValueType() == kNullType
						|| st.GetValues().at(i).GetValueType() == fields[i].GetFieldType())/* ����������ƥ�� */
					{
						records__data.at(i) = st.GetValues().at(i).GetValueData();
					}
					else  {
						std::cout << "�������Ͳ�ƥ�䣡" << endl;
						return false;										/* ͬ���ֶ��������Ͳ�ƥ�䣬������¼ʧ�� */
					}
				}
			}
		}

		/* -----------------------------------------ƥ��ɹ��󣬽����ļ��洢------------------------------------------------- */
		int Record_id = idPool.NewNode();					/* ����¼�¼������ */
		char records_no_[2];
		itoa(Record_id / record_num, records_no_, 10);
		std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no_;/* ��������¼�ļ��� */
		std::string table_name_fields  = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ��� */

		fstream fp;
		fp.open(table_name_records.c_str(),std::ios::app);/* ��or������¼�ļ� */
		/*long l = fp.tellp();
		cout << l << endl;*/
		fp.seekp((Record_id%record_num) * fields.size() * record_len, ios::beg); /* ָ�붨λ������д��λ�� */
		/*l = fp.tellp();
		cout << l << endl;*/

		for (int i = 0; i < fields.size(); i++)
		{
			fp.write(records__data[i].c_str(), record_len);		/* ���ֶ�˳��д���ļ� */
			if (fields[i].IsCreateIndex())					/* ������ֶδ�������ʱ��ά������ */
			{
				int index_id = FindIndex(fields[i].GetFieldName());/* �ҵ����ֶ�index_id��������Ӧ��ţ� */
				if (index_id != -1)
				{
					indexs[index_id].InsertNode(records__data[i].c_str(), Record_id);/* ����������� */
				}
			}
		}
		fp.close();											/* �ر�д��¼�ļ� */

		fp.open(table_name_fields.c_str());
		records_num++;										/* ����ɹ������м�¼������һ */
		char records_numb[4];
		itoa(records_num, records_numb, 10);				/* ���µļ�¼������������ */
		fp.seekp(0, ios::beg);
		fp.write(records_numb, sizeof(char)* 4);
		fp.close();											/* �ر�д��ͷ�ļ� */
		std::cout << "����ɹ���" << endl;
		return true;										/* ���سɹ� */
	}
	else {
		std::cout << "����ʧ�ܣ�" << endl;
		return false;										/* ��Ŀ���ʧ�ܣ�����false */
	}
}

/**
*  \brief ɾ����¼
*/
bool Table::DeleteRecord(SQLDelete &sd)
{
	table_name = sd.GetTableName();			/* ��ȡ������ */
	if (UseTable())
	{/* �򿪱��ɹ� */
		if (!Table::SelectRecord(sd))
		{
			std::cout << "����ʧ�ܣ�" << endl;
			return false;					/* ����Ŀ���¼ʧ�ܣ�����false */
		}
		else {
			int Record_id;					/* ����Ŀ���¼�ɹ���Record_id��¼��ǰҪɾ���ļ�¼���� */
			std::string Null_str = "";		/* ɾ����¼��Ϊ����¼�ÿ� */
			for (int i = 0; i < select_id.size(); i++)
			{/* ��Ҫɾ���������а���ȡ��������ɾ����Ӧ��¼ */
				/* ��delete��SQL���У���IsInputWhere()��trueΪ����ɾ����falseΪȫ��ɾ������ȫ��ɾ����select����Ӧ�����е�����id���������� */
				Record_id = select_id[i];
				char records_no[1], record__data[record_len];
				std::vector<string> records__data;
				itoa(Record_id / record_num, records_no, 10);
				std::string record_file = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no;/* ����Ŀ���ļ��� */
				std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ��� */

				fstream fp;
				fp.open(record_file,  std::ios::in);								/* �򿪶��ļ� */
				fp.seekg((Record_id%record_num)*fields.size() * record_len, ios::beg);/* ָ�붨λ */
				for (int j = 0; j < fields.size(); j++)								/* ��ԭ��¼����records__data */
				{
					fp.read(record__data, sizeof(char)* record_len);
					std::string r;
					records__data.push_back(r);
					records__data[j] = record__data;
				}
				fp.close();															/* �رն��ļ� */

				fp.open(record_file);												/* ��Ŀ���ļ� */
				fp.seekp((Record_id%record_num)*fields.size() * record_len, ios::beg);     /* ��λ��Ҫ���ĵ�λ�� */
				for (int j = 0; j < fields.size(); j++)								/* �޸��ֶ��е�ÿ��ֵ */
				{
					fp.write(Null_str.c_str(), record_len);
					if (fields[j].IsCreateIndex())									/* ����Ӧ�ֶδ�������ʱ������������ά�� */
					{
						int index_id = FindIndex(fields[j].GetFieldName());			/* �ҵ����ֶζ�Ӧ���������index_id */
						if (index_id == -1)
						{
							std::cout << "���ֶβ�����������" << endl;
							return false;
						}
						indexs[index_id].DeleteNode(records__data[j]);				/* ɾ����Ӧ��� */
					}
				}
				fp.close();															/* �ر�д�ļ� */

				fp.open(table_name_fields.c_str());
				records_num--;										/* ����ɹ������м�¼������һ */
				char records_numb[4];
				itoa(records_num, records_numb, 10);				/* ���µļ�¼���ݸ��� */
				fp.seekp(0, ios::beg);
				fp.write(records_numb, sizeof(char)* 4);
				fp.close();
			}
			std::cout << "ɾ���ɹ���" << endl;
			return true;					/* �޸ĳɹ�������true */
		}
	}
	else {
		std::cout << "����ʧ�ܣ�" << endl;
		return false;						/* ����ʧ�ܣ�����false */
	}
}

/**
*  \brief ����/���ļ�¼
*/
bool Table::UpdateRecord(SQLUpdate &su)
{
	table_name = su.GetTableName();								/* ��ȡ������ */
	if (UseTable())												/* �򿪱��ļ��� */
	{
		if (!Table::SelectRecord(su))							/* ������Ҫ���µļ�¼ */
		{
			std::cout << "����ʧ�ܣ�" << endl;
			return false;
		}
		else {
			/* ---------------------------------------------�ж��ֳ����ֶ���----------------------------------------------------- */
			if (fields.size() < su.GetNewField().size())		/* �жϸ��¸�ʽ�Ƿ�ƥ�� */
			{
				std::cout << "�����ֶ������࣡" << endl;
				return false;
			}

			for (int i = 0; i < su.GetNewValue().size(); i++)	/* �жϸ����ֳ��Ƿ�ƥ�� */
			{
				if (su.GetNewValue().at(i).GetValueData().size() >= 255)
				{
					std::cout << "����ĳ��ȹ���" << endl;
					return false;								/* �ַ�������������ʧ�� */
				}
			}

			fstream fwp, frp;
			/* ---------------------------------------------ƥ���ֶ���ֵ----------------------------------------------------- */
			int Record_id;										/* Record_id��¼��ǰ������¼���� */
			for (int i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				char records_no[1], record__data[record_len];			/* record__data��¼һ���ֶε���Ϣ */
				std::vector<string> records__data1;				/* records__data1��¼ԭ�м�¼��Ϣ */
				std::vector<string> records__data2;				/* records__data2��¼��ǰ��Ҫ���ĵļ�¼��Ϣ */

				itoa(Record_id / record_num, records_no, 10);
				std::string record_file = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no;/* ������¼�ļ��� */
				frp.open(record_file.c_str(),  std::ios::in); /* �򿪶���¼�ļ� */
				frp.seekg((Record_id%record_num )*fields.size() * record_len, ios::beg);/* ��λ���ļ�ָ�� */
				for (int j = 0; j < fields.size(); j++)							/* ����Ҫ���ĵļ�¼��Ϣ */
				{
					frp.read(record__data, sizeof(char) * record_len);
					std::string *r1 = new string();
					std::string *r2 = new string();
					records__data1.push_back(*r1);
					records__data2.push_back(*r2);
					records__data1[j] = record__data;
				}
				frp.close();													/* �رն��ļ� */


				for (int j = 0; j < su.GetNewField().size(); j++)
				{
					bool key = false;
					for (int k = 0; k < fields.size(); k++)
					{
						if (su.GetNewField().at(j) == fields[k].GetFieldName())/* �жϸ��¼�¼���ֶ����Ƿ��ƥ�� */
						{
							key = true;
							if (su.GetNewValue().at(j).GetValueType() == fields[k].GetFieldType()/* �ж����������Ƿ�ƥ�� */
								|| su.GetNewValue().at(j).GetValueType() == kNullType)
							{
								records__data2[k] = su.GetNewValue().at(j).GetValueData();
							}
							else {
								std::cout << "�������Ͳ�ƥ�䣡" << endl;
								return false;					/* �������Ͳ�ƥ�䣬����false */
							}
						}
					}
					if (key == false)							/* ���ֶ����޷�ƥ�䣬�򷵻�false */
					{
						std::cout << "�����޷�ƥ����ֶ�����" << endl;
						return false;
					}
				}
				/* -----------------------------------------ƥ��ɹ��󣬽����ļ��洢------------------------------------------------- */
				fwp.open(record_file.c_str());/* ��д��¼�ļ� */
				fwp.seekp((Record_id%record_num)*fields.size() * record_len, ios::beg);/* ָ�붨λ */
				for (int j = 0; j < fields.size(); j++)
				{
					fwp.write(records__data2[j].c_str(), record_len);	/* �����ֶ����н��и��� */
					if (fields[j].IsCreateIndex())				/* �����ֶδ�������ʱ������������ά�� */
					{
						int index_id = FindIndex(fields[j].GetFieldName());/* �ҵ��ֶζ�Ӧ�����ı��index_id */
						if (index_id == -1)
						{
							std::cout << "���ֶβ�����������" << endl;
							return false;
						}
						//indexs[index_id].UpdateNode(records__data2[j], records__data1[j]);/* ����������� */
					}
				}
				fwp.close();									/* �ر�д�ļ� */
			}
			std::cout << "���³ɹ���" << endl;
			return true;										/* ���³ɹ� */
		}
	}
	else {
		std::cout << "����ʧ�ܣ�" << endl;
		return false;											/* �򿪱��ļ�ʧ�ܣ�����false */
	}
}

/**
*  \brief ��������
*/
bool Table::CreateIndex(SQLCreateIndex &si)
{
	if (!UseTable())
	{
		return false;
	}

	ValueType type;
	bool flag = false;
	for (auto iter = fields.begin(); iter != fields.end(); iter++)
	{
		if (iter->GetFieldName() == si.GetField())
		{
			iter->SetIsCreateIndex(true);
			type = iter->GetFieldType();
			flag = true;
			break;
		}
	}

	if (flag == false)
	{
		std::cout << "�ֶ�" << si.GetField() << "������." << std::endl;
		return false;
	}

	string index_path = path + "\\" + table_name + "\\";
	Index temp(si.GetIndex(), si.GetField(), type, index_path);
	string cmd = "md " + index_path + "index";
	system(cmd.c_str());

	indexs.push_back(temp);

	return true;
}

/**
*  \brief �����ʾ
*/
bool Table::Display()
{
	std::string table_record = path + "\\" + table_name + "\\" + table_name + "_records_";
	if (Table::UseTable())
	{
		fstream frp;
		for (int i = 0; i < records_num / record_num; i++)
		{
			char records_no[2];
			itoa(i, records_no, 10);
			std::string table_name_n = table_record + records_no;
			frp.open(table_name_n.c_str(), std::ios::in);
			for (int k = 0; k < record_num; k++)
			{
				for (int j = 0; j < fields.size(); j++)
				{
					char record_data[record_len];
					frp.read(record_data, sizeof(char)* record_len);
					std::cout << fields.at(j).GetFieldName() << ":" << record_data << "   ";
				}
				std::cout << ";" << endl;
			}
			frp.close();
		}

		char records_no[2];
		itoa(records_num / record_num, records_no, 10);
		std::string table_name_n = table_record + records_no;
		frp.open(table_name_n.c_str(), std::ios::in);
		for (int k = 0; k < records_num%record_num; k++)
		{
		
			std::cout << k+1 << endl;
			for (int j = 0; j < fields.size(); j++) 
			{
				char record_data[record_len];
				record_data[0] = '\0';
				frp.read(record_data, sizeof(char)* record_len);
				//frp >> record_data;
				std::cout << fields.at(j).GetFieldName() << ":" << record_data << "  " <<endl;
			}
			
		}
		frp.close();
		return true;
	}
	else {
		std::cout << "�򿪱�ʧ�ܣ�" << endl;
		return false;
	}
}