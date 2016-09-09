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
	//std::cout << "~\(�R���Q)/~" << endl;
	fstream file_stream_,fp;
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file.c_str(), ios::out | ios::binary);
	file_stream_.write((char*)(&idPool), sizeof(idPool));
	file_stream_.close();

	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ��� */
	fp.open(table_name_fields.c_str());
	char records_numb[4];
	itoa(records_num, records_numb, 10);				/* ���µļ�¼������������ */
	fp.seekp(0, ios::beg);
	fp.write(records_numb, sizeof(char)* 4);
	fp.close();											/* �ر�д��ͷ�ļ� */

	fwp.close();										/* �ر�д�ļ� */
	frp.close();										/* �ر�д�ļ� */


	ofstream findex;
	std::string sign;
	std::string index_tree = path + "\\" + table_name + "\\index\\index_tree";
	findex.open(index_tree, ios::binary);

	for (auto iter : indexs)
	{
		if (iter != nullptr)
		{
			if (iter->bplustree_int_ != nullptr)
			{
				sign = "0\0";
				findex.write(sign.c_str(), sizeof(char)* 2);
				findex.write((char*)(iter->bplustree_int_), sizeof(*(iter->bplustree_int_)));
			}
			else if (iter->bplustree_string_ != nullptr)
			{
				sign = "1\0";
				findex.write(sign.c_str(), sizeof(char)* 2);
				findex.write((char*)(iter->bplustree_string_), sizeof(*(iter->bplustree_string_)));
			}
			delete iter;
			iter = nullptr;
		}
	}
	findex.close();
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
	if (fields.size() != 0)										/* �����Ѿ��򿪣�����true */
	{
		return true;
	}

	std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records";/* ��������¼�ļ��� */
	fwp.open(table_name_records.c_str(), ios::in);
	frp.open(table_name_records.c_str());

	fstream fp_fields;
	fp_fields.open(table_name_fields.c_str(), std::ios::in);
	if (!fp_fields.is_open())									/* �����ʧ�ܣ��򷵻�false */
	{
		//std::cout << table_name_fields << endl;
		return false;
	}

	char is_index[2],type[2], field_name[20],records_numb[4];
	fp_fields.read(records_numb, sizeof(char)* 4);				/* ��ȡ��ǰ��¼���� */
	records_num = atoi(records_numb);
	int i = 0;

	while (fp_fields.read(is_index, sizeof(char)* 2))			/* ��ȡ�ֶζ�Ӧ���������ͽ����ڴ� */
	{
		Field temp;
		fp_fields.read(type, sizeof(char)* 2);
		ValueType _type;
		if (type[0] == '0') {
			_type = kIntegerType;
			temp.SetFieldType(kIntegerType);
		}														/* 0-���ͣ�1-�ַ��� */
		else
		{
			_type = kStringType;
			temp.SetFieldType(kStringType);
		}
		fp_fields.read(field_name, sizeof(char)* 20);			/* ��ȡ�ֶ����ƽ����ڴ� */
		temp.SetFieldName(field_name);
		fields.push_back(temp);

		if (is_index[0] == '1')
		{
			Index * index = new Index("", field_name, _type);
			indexs.push_back(index);
		}
	}
	fp_fields.close();											/* �ر��ļ� */

	ifstream findex;
	std::string index_tree = path + "\\" + table_name + "\\index\\index_tree";
	char sign[2];
	findex.open(index_tree,ios::binary);
	int index_no = 0;
	for (auto iter : indexs)
	{
		findex.read(sign, sizeof(char)* 2);
		if (sign[0] == '0')
		{
			findex.read((char*)(iter->bplustree_int_), sizeof(*(iter->bplustree_int_)));
		}
		else if (sign[0] == '1'){
			findex.read((char*)(iter->bplustree_string_), sizeof(*(iter->bplustree_string_)));
		}
	}
	findex.close();

	fstream file_stream_;										/* ���������ڴ�� */
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file, ios::in | ios::binary);
	file_stream_.read((char*)(&idPool), sizeof(idPool));
	file_stream_.close();
	return true;
}

/**
*  \brief �ҵ��ֶζ�Ӧ���������
*/
int Table::FindIndex(std::string index_field_name)
{
	for (int i = 0; i < indexs.size(); i++)
	{
		if (index_field_name == indexs[i]->GetFieldName())
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
	std::string table = path + "\\" + table_name;
	if (!_access(table.c_str(),0))							/* �жϱ��Ƿ��Ѿ����� */
	{
		std::cout << "�Ѵ��ڸñ���" << endl;
		return false;
	}
	else {
		if (!system(cmd.c_str()))
		{
			fields = sql.GetFields();				/* ��ȡ��ͷ���� */
			records_num = 0;

			fstream fp;
			std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ���table_name_fields */
			fp.open(table_name_fields, std::ios::out);					/* ������ͷ�ļ� */

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

			char records_numb[4];										/* ǰ4λд���¼���� */
			itoa(0, records_numb, 10);
			fp.write(records_numb, 4);

			std::string name;
			ValueType type;												/* ��ȡ�ֶζ�Ӧ���������� */
			std::string type_;											/* type_����������� */
			std::string is_index;										/* is_index����Ƿ�������� */
			for (int i = 0; i < fields.size(); i++)						/* д���ͷ���� */
			{
				name = fields[i].GetFieldName() + '\0';
				type = fields[i].GetFieldType();						/* ��ȡ�ֶζ�Ӧ���������� */
				is_index = "0\0";										/* is_index����Ƿ�������� */
				if (type == kIntegerType) type_ = "0\0";				/* ���0�����Σ����1���ַ��� */
				else type_ = "1\0";
				
				fp.write(is_index.c_str(), 2);							/* �������ռ2λ */
				fp.write(type_.c_str(), 2);								/* ��������ռ2λ���ֶ���20λ */
				fp.write(name.c_str(), 20);
			}
			fp.close();													/* �ر��ļ� */

			fstream file_stream_;										/* ���������洢�ص������ļ� */
			std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
			file_stream_.open(pool_file, ios::out | ios::binary);
			file_stream_.close();

			std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records";/* ��������¼�ļ��� */
			fwp.open(table_name_records);								/* ������¼�ļ� */
			fwp.close();

			fwp.open(table_name_records, ios::in);						/* �򿪼�¼�ļ� */
			frp.open(table_name_records);
    /*  if (!frp.is_open()){
        cerr << "no" << endl; 
      }*/
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
	select_id.clear();
	if (sql.IsInputWhere())
	{
		int id;												
		int field_id = Table::FindIndex(sql.GetField());
		if (field_id != -1)
		{
			id = indexs.at(field_id)->SearchNode(sql.GetValue().GetValueData());
		}
		else {

		}

		if (id != -1)
		{//���ڷ��������ļ�¼
			Table::Display(id);
			select_id.push_back(id);
			return true;
		}
		else {//�����ڷ��������ļ�¼
			std::cout << "�����ڷ��������ļ�¼��" << endl;
			return false;
		}
	}
	else {
		Table::Display();
		return true;
	}
}

/**
*  \brief Delete����Select����
*/
bool Table::SelectRecord(SQLDelete &sql)
{
	table_name = sql.GetTableName();
	select_id.clear();
	if (sql.IsInputWhere())
	{
		int id;
		int field_id = Table::FindIndex(sql.GetField());
		if (field_id != -1)
		{
			id = indexs.at(field_id)->SearchNode(sql.GetValue().GetValueData());
		}
		else {

		}

		if (id != -1)
		{//���ڷ��������ļ�¼
			//Table::Display(id);
			select_id.push_back(id);
			return true;
		}
		else {//�����ڷ��������ļ�¼
			std::cout << "�����ڷ��������ļ�¼��" << endl;
			return false;
		}
	}
	else {
		//Table::Display();
		return true;
	}
}

/**
*  \brief Update����Select����
*/
bool Table::SelectRecord(SQLUpdate &sql)
{
	table_name = sql.GetTableName();
	select_id.clear();
	
	int id;
	int field_id = Table::FindIndex(sql.GetWhereField());
	if (field_id != -1)
	{
		id = indexs.at(field_id)->SearchNode(sql.GetWhereValue().GetValueData());
	}
	else {
		
	}

	if (id != -1)
	{//���ڷ��������ļ�¼
		//Table::Display(id);
		select_id.push_back(id);
		return true;
	}
	else {//�����ڷ��������ļ�¼
		std::cout << "�����ڷ��������ļ�¼��" << endl;
		return false;
	}
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
			records__data.push_back("");
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
		//std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no_;/* ��������¼�ļ��� */
		//std::string table_name_fields  = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ��� */

		//fstream fp;
		//fp.open(table_name_records.c_str(),ios::app);				/* �򿪼�¼�ļ� */
		/*long l = fp.tellp();
		cout << l << endl;*/
		//fwp.seekp((Record_id%record_num) * fields.size() * record_len, ios::beg); /* ָ�붨λ������д��λ�� */
		/*l = fp.tellp();
		cout << l << endl;*/

		for (int i = 0; i < fields.size(); i++)
		{
			fwp.seekp(((Record_id%record_num) * fields.size() + i) * record_len, ios::beg); /* ָ�붨λ������д��λ�� */
			fwp.write(records__data[i].c_str(), record_len);	/* ���ֶ�˳��д���ļ� */
			fwp.flush();
			if (fields[i].IsCreateIndex())						/* ������ֶδ�������ʱ��ά������ */
			{
				int index_id = FindIndex(fields[i].GetFieldName());/* �ҵ����ֶ�index_id��������Ӧ��ţ� */
				if (index_id != -1)
				{
					indexs[index_id]->InsertNode(records__data[i], Record_id);/* ����������� */
				}
			}
		}
		//fp.close();										/* �ر�д��¼�ļ� */

		records_num++;										/* ����ɹ������м�¼������һ */
		//fp.open(table_name_fields.c_str());
		//char records_numb[4];
		//itoa(records_num, records_numb, 10);				/* ���µļ�¼������������ */
		//fp.seekp(0, ios::beg);
		//fp.write(records_numb, sizeof(char)* 4);
		//fp.close();										/* �ر�д��ͷ�ļ� */
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
	char record__data[record_len];
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
				//char records_no[1], record__data[record_len];
				std::vector<string> records__data;
				//itoa(Record_id / record_num, records_no, 10);
				//std::string record_file = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no;/* ����Ŀ���ļ��� */
				//std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ��� */

				//fstream fp;
				//fp.open(record_file,  std::ios::in);								/* �򿪶��ļ� */
				frp.seekg((Record_id%record_num)*fields.size() * record_len, ios::beg);/* ָ�붨λ */
				for (int j = 0; j < fields.size(); j++)								/* ��ԭ��¼����records__data */
				{
					frp.read(record__data, sizeof(char)* record_len);
					records__data.push_back(record__data);
				}
				//fp.close();															/* �رն��ļ� */

				//fp.open(record_file);												/* ��Ŀ���ļ� */
				fwp.seekp((Record_id%record_num)*fields.size() * record_len, ios::beg);/* ��λ��Ҫ���ĵ�λ�� */
				for (int j = 0; j < fields.size(); j++)								/* �޸��ֶ��е�ÿ��ֵ */
				{
					fwp.write(Null_str.c_str(), record_len);
					fwp.flush();
					if (fields[j].IsCreateIndex())									/* ����Ӧ�ֶδ�������ʱ������������ά�� */
					{
						int index_id = FindIndex(fields[j].GetFieldName());			/* �ҵ����ֶζ�Ӧ���������index_id */
						if (index_id == -1)
						{
							std::cout << "���ֶβ�����������" << endl;
							return false;
						}
						indexs[index_id]->DeleteNode(records__data[j]);				/* ɾ����Ӧ��� */
					}
				}
				//fp.close();															/* �ر�д�ļ� */

				//fp.open(table_name_fields.c_str());
				records_num--;										/* ����ɹ������м�¼������һ */
				//char records_numb[4];
				//itoa(records_num, records_numb, 10);				/* ���µļ�¼���ݸ��� */
				//fp.seekp(0, ios::beg);
				//fp.write(records_numb, sizeof(char)* 4);
				//fp.close();
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
	char record__data[record_len];
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

			//fstream fwp, frp;
			/* ---------------------------------------------ƥ���ֶ���ֵ----------------------------------------------------- */
			int Record_id;										/* Record_id��¼��ǰ������¼���� */
			for (int i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				//char records_no[1], record__data[record_len];	/* record__data��¼һ���ֶε���Ϣ */
				std::vector<string> records__data1;				/* records__data1��¼ԭ�м�¼��Ϣ */
				std::vector<string> records__data2;				/* records__data2��¼��ǰ��Ҫ���ĵļ�¼��Ϣ */

				//itoa(Record_id / record_num, records_no, 10);
				//std::string record_file = path + "\\" + table_name + "\\" + table_name + "_records_" + records_no;/* ������¼�ļ��� */
				//frp.open(record_file.c_str(),  std::ios::in); /* �򿪶���¼�ļ� */
				frp.seekg((Record_id%record_num )*fields.size() * record_len, ios::beg);/* ��λ���ļ�ָ�� */
				for (int j = 0; j < fields.size(); j++)							/* ����Ҫ���ĵļ�¼��Ϣ */
				{
					std::string r1, r2;
					frp.read(record__data, sizeof(char) * record_len);
					records__data1.push_back(r1);
					records__data2.push_back(r2);
					records__data1[j] = record__data;
				}
				//frp.close();													/* �رն��ļ� */


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
				//fwp.open(record_file.c_str());/* ��д��¼�ļ� */
				fwp.seekp((Record_id%record_num)*fields.size() * record_len, ios::beg);/* ָ�붨λ */

				int index_id;
				for (int j = 0; j < fields.size(); j++)
				{
					fwp.write(records__data2[j].c_str(), record_len);	/* �����ֶ����н��и��� */
					fwp.flush();
					if (fields[j].IsCreateIndex())				/* �����ֶδ�������ʱ������������ά�� */
					{
						index_id = FindIndex(fields[j].GetFieldName());/* �ҵ��ֶζ�Ӧ�����ı��index_id */
						if (index_id == -1)
						{
							std::cout << "���ֶβ�����������" << endl;
							return false;
						}
						//indexs[index_id].UpdateNode(records__data2[j], records__data1[j]);/* ����������� */
					}
				}
				//fwp.close();									/* �ر�д�ļ� */
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
	int i = 0;
	for (auto iter = fields.begin(); iter != fields.end(); iter++)
	{
		if (iter->GetFieldName() == si.GetField())
		{
			iter->SetIsCreateIndex(true);
			type = iter->GetFieldType();
			flag = true;
			break;
		}
		i++;
	}

	if (flag == false)
	{
		std::cout << "�ֶ�" << si.GetField() << "������." << std::endl;
		return false;
	}

	string index_path = path + "\\" + table_name + "\\index";
	string cmd = "md " + index_path;
	system(cmd.c_str());
	index_path = index_path + "\\" + si.GetField();
	Index * temp = new Index(si.GetIndex(), si.GetField(), type, index_path);

	fstream fip;
	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ���table_name_fields */
	std::string is_index = "1\0";							/* is_index����Ƿ�������� */
	fip.open(table_name_fields.c_str());
	fip.seekp(sizeof(char)*(i * 24 + 4), std::ios::beg);
	fip.write(is_index.c_str(), sizeof(char)* 2);
	fip.close();

	int k = 0;
	char record_field_data[record_len];
	if (records_num != 0)
	{
		while (k < records_num)
		{
			frp.seekg((k*fields.size() + i)*record_len*sizeof(char), ios::beg);
			frp.read(record_field_data, sizeof(char)*record_len);
			temp->InsertNode(record_field_data, k);
			k++;
		}
	}
	indexs.push_back(temp);

	std::cout << "����" << si.GetIndex() << "�����ɹ�." << std::endl;
	return true;
}

/**
*  \brief �����ʾ
*/
bool Table::Display()
{
	//std::string table_record = path + "\\" + table_name + "\\" + table_name + "_records_";
	char record_data[record_len];
	if (Table::UseTable())
	{
		//fstream frp;
		/*for (int i = 0; i < records_num / record_num; i++)
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
		}*/

		/*char records_no[2];
		itoa(records_num / record_num, records_no, 10);
		std::string table_name_n = table_record + records_no;*/
		//frp.open(table_name_n.c_str(), std::ios::in);
		frp.seekg(0, ios::beg);										/* ��λ���ļ���ͷ */
		for (int k = 0; k < records_num%record_num; k++)
		{
		
			std::cout << k+1 << endl;
			for (int j = 0; j < fields.size(); j++) 
			{
				//record_data[0] = '\0';
				frp.read(record_data, sizeof(char)* record_len);
				//frp >> record_data;
				std::cout << fields.at(j).GetFieldName() << ":" << record_data << "  " <<endl;
			}
			
		}
		//frp.close();
		return true;
	}
	else {
		std::cout << "�򿪱�ʧ�ܣ�" << endl;
		return false;
	}
}

/**
*  \brief �����ʾ
*/
bool Table::Display(int id)
{
	char record_data[record_len];
	//record_data[0] = '\0';
	/*std::string table_record = path + "\\" + table_name + "\\" + table_name + "_records_";
	fstream frp;*/
	if (UseTable())
	{
		/*char records_no[2];
		itoa(id, records_no, 10);
		std::string table_name_n = table_record + records_no;
		frp.open(table_name_n.c_str(), std::ios::in);*/

		frp.seekg(sizeof(char)*id*fields.size()*record_len, ios::beg);
		for (int j = 0; j < fields.size(); j++)
		{
			//frp.sync();
			frp.read(record_data, sizeof(char)* record_len);
			//frp >> record_data;
			std::cout << fields.at(j).GetFieldName() << ":" << record_data << "  " << endl;
		}
		return true;
	}
	else {
		std::cout << "����ʧ�ܣ�" << endl;
		return false;
	}
}