#include<iostream>
#include<string>
#include<fstream>

#define record_num 1000

#include "database.h"
#include "table.h"
#include "sqlstatement.h"
#include "MemPool.h"
using namespace std;


/**
*  \brief ���캯��
*/
Table::Table() {}

/**
*  \brief ��������
*/
Table::~Table() {}

/**
*  \brief ��Ŀ������ɹ�����true��ʧ�ܷ���false
*/
bool Table::UseTable()
{
	std::string cmd = "start ..\\" + table_name;					/* ��������򿪱��ļ��� */
	if (system(cmd.c_str()))
	{
		std::string table_name_fields = table_name + "_fields";		/* ��������򿪱�ͷ�ļ� */
		fstream fp_fields;
		fp_fields.open(table_name_fields.c_str(), std::ios::binary | std::ios::in);

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
	else return false;
}

/**
*  \brief �����±�
*/
bool Table::CreateTable(SQLCreateTable &sql)
{
	table_name = sql.GetTableName();	/* ��ñ����� */
	std::string cmd = "md ..\\" + table_name;		/* ������������ļ��� */
	if (Table::UseTable())							/* �жϱ��Ƿ��Ѿ����� */
	{
		return false;
	}
	else {
		if (system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* ��ȡ��ͷ���� */
			
			Table::UseTable();						/* �򿪱��ļ��� */
			fstream fp;
			std::string table_name_fields = table_name + "_fields";						/* ������ͷ�ļ���table_name_fields */
			fp.open(table_name_fields.c_str(), std::ios::binary | std::ios::out);		/* ������ͷ�ļ� */

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
			return true;
		}
		else return false;
	}
}

/**
*  \brief �����¼�¼
*/
bool Table::CreateRecord(SQLInsert &st)
{
	if (records_num >= record_num - 1) return false;		/* �жϵ�ǰ���Ƿ����� */

	if (Table::UseTable())									/* ��Ŀ����ļ� */
	{
		int Record_id = MemPool::NewNode();					/* ����¼�¼������ */
		char records_no_[1];
		itoa(Record_id / record_num, records_no_, 1);
		std::string table_name_records = st.GetTableName() + "_records_" + records_no_;	/* ��������¼�ļ��� */

		fstream fp;
		fp.open(table_name_records.c_str(), std::ios::binary | std::ios::out);/* ��or������¼�ļ� */

		if (st.IsInputField())								
		{/* �������valueָ������Ӧ�ֶ�ʱ */
			int key;										/* key�жϲ����Ƿ�ɹ� */
			for (int i = 0; i < st.GetFields().size(); i++) /* ����ƥ���ƶ��ֶ����ͷ�ֶ� */
			{
				key = 0;
				for (int j = 0; j < fields.size(); j++)
				{
					if (st.GetFields().at(i) == fields[j].GetFieldName())	/* ƥ�䵽ͬ���ֶ�key = 1 */
					{
						key = 1;
						if (st.GetValues().at(i).GetValueType() == fields[j].GetFieldType())/* ����������Ҳƥ�䣬����ָ��λ��д���ļ� */
						{
							fp.seekp(((Record_id%record_num - 1) * fields.size() + j) * 255, ios::beg);
							fp.write(st.GetValues().at(i).GetValueData().c_str(), 255);
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
				for (int i = 0; i < st.GetValues().size(); i++)				/* ��Ĭ��˳�����value */
				{
					if (st.GetValues().at(i).GetValueType() == fields[i].GetFieldType())/* ����������ƥ�� */
					{
						fp.seekp(((Record_id%record_num - 1) * fields.size() + i) * 255, ios::beg);/* ��ָ��λ��д���¼���� */
						fp.write(st.GetValues().at(i).GetValueData().c_str(), 255);
					}
					else return false;										/* �������Ͳ�ƥ��ʱ������ʧ�� */
				}
			}
		}
		records_num++;										/* ����ɹ������м�¼������һ */
		return true;										/* ���سɹ� */
	}
	else return false;										/* ��Ŀ���ʧ�ܣ�����false */
}