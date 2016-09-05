#include<iostream>
#include<string>
#include<fstream>

#include "database.h"
#include "table.h"
#include "sqlstatement.h"
#include "IDPool.h"
using namespace std;


/**
*  \brief ���캯��
*/
Table::Table(std::string new_path) 
{
	path = new_path;			/* ��ȡ���洢·�� */
}

/**
*  \brief ��������
*/
Table::~Table() 
{

}

/**
*  \brief ��Ŀ������ɹ�����true��ʧ�ܷ���false
*/
bool Table::UseTable()
{
	std::string table_name_fields = path + table_name + "\\" + table_name + "_fields";/* ��������򿪱�ͷ�ļ� */
	fstream fp_fields;
	fp_fields.open(table_name_fields.c_str(), std::ios::binary | std::ios::in);
	if (fp_fields.bad())										/* �����ʧ�ܣ��򷵻�false */
	{
		return false;
	}

	char type[2],field_name[20];
	int i = 0;
	while (fp_fields.read(type, sizeof(char)* 2))				/* ��ȡ�ֶζ�Ӧ���������ͽ����ڴ� */
	{
		if (type[0] == '0') fields[i].SetFieldType(kIntegerType);/* 0-���ͣ�1-�ַ��� */
		else fields[i].SetFieldType(kStringType);
		fp_fields.read(field_name, sizeof(char)* 20);			/* ��ȡ�ֶ����ƽ����ڴ� */
		fields[i].SetFieldName(field_name);
	}
	fp_fields.close();											/* �ر��ļ� */
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

	std::string cmd = "md " + path + table_name;	/* ������������ļ��� */
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
			std::string table_name_fields = path + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ���table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::binary | std::ios::out);		/* ������ͷ�ļ� */

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

			for (int i = 0; i < fields.size(); i++)						/* д���ͷ���� */
				{
				std::string name = fields[i].GetFieldName() + '\0';
				ValueType type = fields[i].GetFieldType();				/* ��ȡ�ֶζ�Ӧ���������� */
				std::string type_;										/* type_����������� */
				if (type == kIntegerType) type_ = "0\0";				/* ���0�����Σ����1���ַ��� */
				else type_ = "1\0";
				fp.write(type_.c_str(), 2);								/* ��������ռ2λ���ֶ���20λ */
				fp.write(name.c_str(),20);
			}
			fp.close();													/* �ر��ļ� */
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
	int field_id = Table::FindIndex(sql.GetField());
	
}

/**
*  \brief Delete����Select����
*/
bool Table::SelectRecord(SQLDelete &sd)
{
	table_name = sd.GetTableName();
	int field_id = Table::FindIndex(sd.GetField());

}

/**
*  \brief Update����Select����
*/
bool Table::SelectRecord(SQLUpdate &su)
{
	table_name = su.GetTableName();
	int field_id = Table::FindIndex(su.GetWhereField());
}

/**
*  \brief �����¼�¼
*/
bool Table::CreateRecord(SQLInsert &st)
{
	table_name = st.GetTableName();							/* ��ȡ������ */
	if (records_num >= record_num - 1) return false;		/* �жϵ�ǰ���Ƿ����� */

	if (Table::UseTable())									/* ��Ŀ����ļ� */
	{
		std::vector<string> records__data;					/* records__data�洢Ҫ��������� */

		/* ---------------------------------------------ƥ���ֶ���ֵ----------------------------------------------------- */
		if (st.IsInputField())								
		{/* �������valueָ������Ӧ�ֶ�ʱ */
			for (int i = 0; i < st.GetValues().size(); i++)						/* �ж��ַ������� */
			{
				if (st.GetValues().at(i).GetValueData().size() >= 255)			/* ���ַ�����������洢ʧ�� */
				{
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
							records__data[j] = st.GetValues().at(i).GetValueData();
						}
						else return false;									/* ͬ���ֶ��������Ͳ�ƥ�䣬������¼ʧ�� */
					}
				}
				if (key == 0) return false;					/* ��ǰ�ֶ���δƥ�䵽ͬ����ͷ�ֶΣ�����ʧ�� */
			}
		}
		else {/* �������valueδָ����Ӧ�ֶ�ʱ */
			if (st.GetValues().size() > fields.size()) return false;		/* ������value��Ŀ�����ֶ����������ʧ�� */
			else {
				for (int i = 0; i < st.GetValues().size(); i++)						/* �ж��ַ������� */
				{
					if (st.GetValues().at(i).GetValueData().size() >= 255)			/* ���ַ�����������洢ʧ�� */
					{
						return false;
					}
				}

				for (int i = 0; i < st.GetValues().size(); i++)				/* ��Ĭ��˳�����value */
				{	
					if (st.GetValues().at(i).GetValueType() == kNullType 
						|| st.GetValues().at(i).GetValueType() == fields[i].GetFieldType())/* ����������ƥ�� */
					{
						records__data[i] = st.GetValues().at(i).GetValueData();
					}
					else return false;										/* �������Ͳ�ƥ��ʱ������ʧ�� */
				}
			}
		}

		/* -----------------------------------------ƥ��ɹ��󣬽����ļ��洢------------------------------------------------- */
		int Record_id = idPool.NewNode();					/* ����¼�¼������ */
		char records_no_[1];
		itoa(Record_id / record_num, records_no_, 1);
		std::string table_name_records = path + st.GetTableName() + "\\" + st.GetTableName() + "_records_" + records_no_;/* ��������¼�ļ��� */

		fstream fp;
		fp.open(table_name_records.c_str(), std::ios::binary | std::ios::out);/* ��or������¼�ļ� */
		fp.seekp((Record_id%record_num - 1) * fields.size() * 255, ios::beg); /* ָ�붨λ������д��λ�� */

		for (int i = 0; i < fields.size(); i++)
		{
			fp.write(records__data[i].c_str(), 255);		/* ���ֶ�˳��д���ļ� */
			if (fields[i].IsCreateIndex())					/* ������ֶδ�������ʱ��ά������ */
			{
				int index_id = FindIndex(fields[i].GetFieldName());/* �ҵ����ֶ�index_id��������Ӧ��ţ� */
				indexs[index_id].InsertNode(records__data[i].c_str(), Record_id);/* ����������� */
			}
		}

		records_num++;										/* ����ɹ������м�¼������һ */
		return true;										/* ���سɹ� */
	}
	else return false;										/* ��Ŀ���ʧ�ܣ�����false */
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
			return false;					/* ����Ŀ���¼ʧ�ܣ�����false */
		}
		else {
			int Record_id;					/* ����Ŀ���¼�ɹ���Record_id��¼��ǰҪɾ���ļ�¼���� */
			std::string Null_str = "";		/* ɾ����¼��Ϊ����¼�ÿ� */
			for (int i = 0; i < select_id.size(); i++)
			{/* ��Ҫɾ���������а���ȡ��������ɾ����Ӧ��¼ */
			 /* ��delete��SQL���У���IsInputWhere()��trueΪ����ɾ����falseΪȫ��ɾ������ȫ��ɾ����select����Ӧ�����е�����id���������� */
				Record_id = select_id[i];
				char records_no[1],record__data[255];
				std::vector<string> records__data;
				itoa(Record_id / record_num, records_no, 1);						
				std::string record_file = path + table_name + "\\" + table_name + "_records_" + records_no;/* ����Ŀ���ļ��� */

				fstream fp;
				fp.open(record_file, std::ios::binary | std::ios::in);				/* �򿪶��ļ� */
				fp.seekg((Record_id%record_num - 1)*fields.size() * 255, ios::beg); /* ָ�붨λ */
				for (int j = 0; j < fields.size(); j++)								/* ��ԭ��¼����records__data */
				{
					fp.read(record__data, sizeof(char)* 255);
					std::string *r = new string();
					records__data.push_back(*r);
					records__data[j] = record__data;
				}
				fp.close();															/* �رն��ļ� */
				
				fp.open(record_file, std::ios::binary | std::ios::out);				/* ��Ŀ���ļ� */
				fp.seekp((Record_id%record_num - 1)*fields.size() * 255, ios::beg); /* ��λ��Ҫ���ĵ�λ�� */
				for (int j = 0; j < fields.size(); j++)								/* �޸��ֶ��е�ÿ��ֵ */
				{
					fp.write(Null_str.c_str(), 255);
					if (fields[j].IsCreateIndex())									/* ����Ӧ�ֶδ�������ʱ������������ά�� */
					{
						int index_id = FindIndex(fields[j].GetFieldName());			/* �ҵ����ֶζ�Ӧ���������index_id */
						indexs[index_id].DeleteNode(records__data[j]);				/* ɾ����Ӧ��� */
					}
				}
				fp.close();															/* �ر�д�ļ� */
			}
			records_num--;					/* ɾ���ɹ������м�¼������һ */
			return true;					/* �޸ĳɹ�������true */
		}
	}
	else return false;						/* ����ʧ�ܣ�����false */
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
			return false;
		}
		else {
		/* ---------------------------------------------�ж��ֳ����ֶ���----------------------------------------------------- */
			if (fields.size() < su.GetNewField().size())		/* �жϸ��¸�ʽ�Ƿ�ƥ�� */
			{
				return false;
			}

			for (int i = 0; i < su.GetNewValue().size(); i++)	/* �жϸ����ֳ��Ƿ�ƥ�� */
			{
				if (su.GetNewValue().at(i).GetValueData().size() >= 255)
				{
					return false;								/* �ַ�������������ʧ�� */
				}
			}

			fstream fwp,frp;
		/* ---------------------------------------------ƥ���ֶ���ֵ----------------------------------------------------- */
			int Record_id;										/* Record_id��¼��ǰ������¼���� */
			for (int i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				char records_no[1], record__data[256];			/* record__data��¼һ���ֶε���Ϣ */
				std::vector<string> records__data1;				/* records__data1��¼ԭ�м�¼��Ϣ */
				std::vector<string> records__data2;				/* records__data2��¼��ǰ��Ҫ���ĵļ�¼��Ϣ */

				itoa(Record_id / record_num, records_no, 1);
				std::string record_file = path + table_name + "\\" + table_name + "_records_" + records_no;/* ������¼�ļ��� */
				frp.open(record_file.c_str(), std::ios::binary | std::ios::in); /* �򿪶���¼�ļ� */
				frp.seekg((Record_id%record_num - 1)*fields.size() * 255, ios::beg);/* ��λ���ļ�ָ�� */
				for (int j = 0; j < fields.size(); j++)							/* ����Ҫ���ĵļ�¼��Ϣ */
				{
					frp.read(record__data, sizeof(char)* 255);
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
								return false;					/* �������Ͳ�ƥ�䣬����false */
							}
						}
					}
					if (key == false)							/* ���ֶ����޷�ƥ�䣬�򷵻�false */
					{
						return false;
					}
				}
		/* -----------------------------------------ƥ��ɹ��󣬽����ļ��洢------------------------------------------------- */
				fwp.open(record_file.c_str(), std::ios::binary | std::ios::out);/* ��д��¼�ļ� */
				fwp.seekp((Record_id%record_num - 1)*fields.size() * 255, ios::beg);/* ָ�붨λ */
				for (int j = 0; j < fields.size(); j++)
				{
					fwp.write(records__data2[j].c_str(), 255);	/* �����ֶ����н��и��� */
					if (fields[j].IsCreateIndex())				/* �����ֶδ�������ʱ������������ά�� */
					{
						int index_id = FindIndex(fields[j].GetFieldName());/* �ҵ��ֶζ�Ӧ�����ı��index_id */
						//indexs[index_id].UpdateNode(records__data2[j], records__data1[j]);/* ����������� */
					}
				}
				fwp.close();									/* �ر�д�ļ� */
			}
			return true;										/* ���³ɹ� */
		}
	}
	else{
		return false;											/* �򿪱��ļ�ʧ�ܣ�����false */
	}
}

/**
*  \brief ��������
*/
bool Table::CreateIndex(SQLCreateIndex &si)
{
	
}