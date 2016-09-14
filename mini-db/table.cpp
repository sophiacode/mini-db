#include<iostream>
#include<string>
#include<fstream>

#include "database.h"
#include "table.h"
#include "sqltype.h"

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
	ofstream file_stream_,fp;
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file.c_str(), ios::in |ios::out| ios::binary);
	file_stream_.seekp(0, ios::beg);
	file_stream_.write((char*)(idPool), sizeof(*idPool));
	file_stream_.close();

	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ��� */
	fp.open(table_name_fields,ios::binary|ios::in);
	char records_numb[10];
	itoa(records_num, records_numb, 10);				/* ���µļ�¼������������ */
	fp.seekp(0, ios::beg);
	fp.write(records_numb, sizeof(char)* 11);
    fp.flush();
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
				iter->bplustree_int_->CloseTree();
				sign = "0\0";
				findex.write(sign.c_str(), sizeof(char)* 2);
				findex.write((char*)(iter->bplustree_int_), sizeof(*(iter->bplustree_int_)));
			}
			else if (iter->bplustree_string_ != nullptr)
			{
				iter->bplustree_string_->CloseTree();
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
	fwp.open(table_name_records, ios::in | ios::binary);
	frp.open(table_name_records, ios::binary);

	ifstream fp_fields;
	fp_fields.open(table_name_fields, ios::in | ios::binary);
	if (!fp_fields.is_open())									/* �����ʧ�ܣ��򷵻�false */
	{
		return false;
	}

	char is_index[2], type[2], field_name[20], records_numb[11], fields_numb[11];
	fp_fields.seekg(0, ios::beg);
	fp_fields.read(records_numb, sizeof(records_numb));				/* ��ȡ��ǰ��¼���� */
	records_num = atoi(records_numb);
	fp_fields.seekg(sizeof(char)* 16, ios::beg);
	fp_fields.read(fields_numb, sizeof(fields_numb));				/* ��ȡ�ֶ��� */
	USER_INT fields_num = atoi(fields_numb);
	USER_INT i = 0;
	record_leng = 0;

	Index * id_index = new Index("id_index", "id_index", kIntegerType);
	indexs.push_back(id_index);

	while (i <fields_num)										/* ��ȡ�ֶζ�Ӧ���������ͽ����ڴ� */
	{
		Field temp;
		ValueType _type;
		fp_fields.seekg((32 + 24 * i)*sizeof(char), ios::beg);
		fp_fields.read(is_index, sizeof(char)* 2);
		fp_fields.read(type, sizeof(char)* 2);
		if (type[0] == '0') {
			_type = kIntegerType;
			temp.SetFieldType(kIntegerType);
			record_leng += true_int;
		}														/* 0-���ͣ�1-�ַ��� */
		else
		{
			_type = kStringType;
			temp.SetFieldType(kStringType);
			record_leng += true_len;
		}
		fp_fields.read(field_name, sizeof(char)* 20);			/* ��ȡ�ֶ����ƽ����ڴ� */
		temp.SetFieldName(field_name);
		fields.push_back(temp);

		if (is_index[0] == '1')
		{
			Index * index = new Index("", field_name, _type);
			indexs.push_back(index);
		}
		i++;
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

    idPool = new IDPool();
	ifstream file_stream_;										/* ���������ڴ�� */
	std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
	file_stream_.open(pool_file, ios::in | ios::binary);
	file_stream_.seekg(0, ios::beg);
	file_stream_.read((char*)(idPool), sizeof(*idPool));
	file_stream_.close();
	return true;
}

/**
*  \brief �ҵ��ֶζ�Ӧ���������
*/
int Table::FindIndex(std::string index_field_name)
{
	for (int i = 1; i < indexs.size(); i++)
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

	fields = sql.GetFields();									/* ��ȡ��ͷ���� */
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

	std::string cmd = "md " + path + "\\" + table_name;		/* ������������ļ��� */
	std::string table = path + "\\" + table_name;
	if (!_access(table.c_str(),0))							/* �жϱ��Ƿ��Ѿ����� */
	{
		std::cout << "�Ѵ��ڸñ���" << endl;
		return false;
	}
	else {
		if (!system(cmd.c_str()))
		{
			records_num = 0;
			record_leng = 0;

			fstream fp;
			std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ���table_name_fields */
			fp.open(table_name_fields, std::ios::out|ios::binary);		/* ������ͷ�ļ� */
																		
			char records_numb[11], fields_numb[11];						/* ��16λд���ֶ�����ǰ16λд���¼���� */
			itoa(0, records_numb, 10);
			fp.seekp(0, ios::beg);
			fp.write(records_numb, sizeof(records_numb));
			itoa(fields.size(), fields_numb, 10);
			fp.seekp(16, ios::beg);
			fp.write(fields_numb, sizeof(fields_numb));
			fp.flush();

			std::string name;
			ValueType type;												/* ��ȡ�ֶζ�Ӧ���������� */
			std::string type_;											/* type_����������� */
			std::string is_index;										/* is_index����Ƿ�������� */
			for (int i = 0; i < fields.size(); i++)						/* д���ͷ���� */
			{
				name = fields[i].GetFieldName() + '\0';
				type = fields[i].GetFieldType();						/* ��ȡ�ֶζ�Ӧ���������� */
				is_index = "0\0";										/* is_index����Ƿ�������� */
				if (type == kIntegerType)
				{
					type_ = "0\0";										/* ���0�����Σ����1���ַ��� */
					record_leng += true_int;
				}
				else {
					type_ = "1\0";
					record_leng += true_len;
				}
				
				fp.seekp((32 + i * 24)*sizeof(char), ios::beg);
				fp.write(is_index.c_str(), 2);							/* �������ռ2λ */
				fp.write(type_.c_str(), 2);								/* ��������ռ2λ���ֶ���20λ */
				fp.write(name.c_str(), 20);
			}
			fp.close();													/* �ر��ļ� */

      
			fstream file_stream_;										/* ���������洢�ص������ļ� */
			std::string pool_file = path + "\\" + table_name + "\\" + table_name + "_idPool";
			idPool = new IDPool(pool_file);
			file_stream_.open(pool_file, ios::out | ios::binary);
			file_stream_.close();

			std::string table_name_records = path + "\\" + table_name + "\\" + table_name + "_records";/* ��������¼�ļ��� */
			fwp.open(table_name_records,ios::binary);								/* ������¼�ļ� */
			fwp.close();

			fwp.open(table_name_records, ios::in|ios::binary);						/* �򿪼�¼�ļ� */
			frp.open(table_name_records);

			string index_path = path + "\\" + table_name + "\\index";
			if (_access(index_path.c_str(), 0))
			{
				string cmd = "md " + index_path;
				system(cmd.c_str());
			}
			index_path = index_path + "\\id_index";
			Index * temp = new Index("id_index", "id_index", kIntegerType, index_path);
			indexs.push_back(temp);

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
	if (sql.IsInputWhere())																	/* ��select����where�Ӿ䣬��ʾѡ�м�¼ */
	{											
		int field_id = Table::FindIndex(sql.GetField());									/* �����ֶζ�Ӧ������id */
		if (field_id != -1)
		{
			for (int i = 0; i < fields.size(); i++)
			{
				if (fields.at(i).GetFieldName() == sql.GetField())
				{
					if (sql.GetValue().GetValueType() != kNullType 
						&& fields.at(i).GetFieldType() != sql.GetValue().GetValueType())
					{
						std::cout << "���������޷�ƥ�䣡" << endl;
						return false;
					}
				}
			}

			if (indexs.at(field_id)->
				SearchNode(sql.GetValue().GetValueData(), 
					       select_id, sql.GetOperatorType()) == true)/* ������������ */
			{
				for (USER_INT iter = 0; iter < select_id.size();iter++)						/* ���ҳɹ� */
				{
					Display(select_id[iter],iter);
				}
				std::cout << endl;
				return true;
			}
			else
			{
				std::cout << "�����ڷ��������ļ�¼��" << endl;	/* ����ʧ�� */
				return false;
			}
		}
		else {
			if (OrderSelect(sql.GetField(), sql.GetValue(), sql.GetOperatorType()))
			{
				for (USER_INT iter = 0; iter < select_id.size(); iter++)						/* ���ҳɹ� */
				{
					Display(select_id[iter], iter);
				}
				std::cout << endl;
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {/* select ������where�Ӿ䣬��ʾȫ���¼ */
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
	if (sql.IsInputWhere())								/* ��delete������where�Ӿ� */
	{
		int field_id = Table::FindIndex(sql.GetField());/* ����������Ӧ��� */
		if (field_id != -1)								/* �������������������� */
		{
			for (int i = 0; i < fields.size(); i++)
			{
				if (fields.at(i).GetFieldName() == sql.GetField())
				{
					if (fields.at(i).GetFieldType() != sql.GetValue().GetValueType())
					{
						std::cout << "�������Ͳ����ϣ�" << endl;
						return false;
					}
				}
			}

			if (indexs.at(field_id)->
				SearchNode(sql.GetValue().GetValueData(),
					select_id, sql.GetOperatorType()) == true)
			{
				return true;
			}
			else
			{
				std::cout << "�����ڷ��������ļ�¼��" << endl;
				return false;
			}
		}
		else {											/* ������������˳����� */
			if (OrderSelect(sql.GetField(), sql.GetValue(), kOpEqual))
			{
				return true;
			}
			else {
				return false;
			}
		}

	}
	else {
		char record__data[record_len];
		USER_INT k = 0, i = 0;
		while (k < records_num)
		{
			bool key = false;
			for (int j = 0; j < fields.size(); j++)
			{
				record__data[0] = '\0';
				frp.seekg(sizeof(char)*true_len*(i*fields.size() + j), ios::beg);
				frp.read(record__data, sizeof(char)* record_len);
				//frp >> record__data;
				if (record__data[0] != '\a')
				{
					key = true;
					break;
				}
			}

			if (key == true)
			{
				select_id.push_back(i);
				k++;
			}
			i++;
		}
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
	int field_id = Table::FindIndex(sql.GetWhereField());	/* ����������Ӧ���id */
	if (field_id != -1)
	{														/* ���������������������� */
		for (int i = 0; i < fields.size(); i++)
		{
			if (fields.at(i).GetFieldName() == sql.GetWhereField())
			{
				if (fields.at(i).GetFieldType() != sql.GetWhereValue().GetValueType())
				{
					std::cout << "�������Ͳ����ϣ�" << endl;
					return false;
				}
			}
		}

		if (indexs.at(field_id)->
			SearchNode(sql.GetWhereValue().GetValueData(),
				select_id, sql.GetOperatorType()) == true)
		{
			return true;
		}
		else
		{
			std::cout << "�����ڷ��������ļ�¼��" << endl;
			return false;
		}
	}
	else {													/* ������������˳����� */
		if (OrderSelect(sql.GetWhereField(), sql.GetWhereValue(), kOpEqual))
		{
			return true;
		}
		else {
			return false;
		}
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
			records__data.push_back("\a");
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
		USER_INT Record_id = idPool->NewNode();					/* ����¼�¼������ */
		USER_INT offset = 0;
		
		char id_char[20];
		itoa(Record_id, id_char, 10);
		indexs[0]->InsertNode(id_char, Record_id);

		for (int i = 0; i < fields.size(); i++)
		{
			fwp.seekp(Record_id*record_leng + offset, ios::beg); /* ָ�붨λ������д��λ�� */
			if (fields.at(i).GetFieldType() == kIntegerType)
			{
				fwp.write(records__data[i].c_str(), int_len);	/* ���ֶ�˳��д���ļ� */
				offset += true_int;
			}
			else {
				fwp.write(records__data[i].c_str(), record_len);/* ���ֶ�˳��д���ļ� */
				offset += true_len;
			}
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

		records_num++;										/* ����ɹ������м�¼������һ */
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
			return false;					/* ����Ŀ���¼ʧ�ܣ�����false */
		}
		else {
			USER_INT Record_id;					/* ����Ŀ���¼�ɹ���Record_id��¼��ǰҪɾ���ļ�¼���� */
			USER_INT offset;
			std::string Null_str = "\a";		/* ɾ����¼��Ϊ����¼�ÿ� */
			for (USER_INT i = 0; i < select_id.size(); i++)
			{/* ��Ҫɾ���������а���ȡ��������ɾ����Ӧ��¼ */
				/* ��delete��SQL���У���IsInputWhere()��trueΪ����ɾ����falseΪȫ��ɾ������ȫ��ɾ����select����Ӧ�����е�����id���������� */
				Record_id = select_id[i];
				offset = 0;
				std::vector<string> records__data;

				for (int j = 0; j < fields.size(); j++)				/* ��ԭ��¼����records__data */
				{
					frp.seekg(Record_id*record_leng + offset, ios::beg);/* ָ�붨λ */
					frp >> record__data;
					records__data.push_back(record__data);

					fwp.seekp(Record_id*record_leng + offset, ios::beg);/* ָ�붨λ */
					if (fields.at(j).GetFieldType() == kIntegerType)
					{
						fwp.write(Null_str.c_str(), int_len);
						offset += true_int;
					}
					else {
						fwp.write(Null_str.c_str(), record_len);
						offset += true_len;
					}
					fwp.flush();
				}
				
				for (int j = 0; j < fields.size(); j++)								/* �޸��ֶ��е�ÿ��ֵ */
				{
					//fwp.seekp((Record_id*fields.size() + j) *true_len, ios::beg);/* ָ�붨λ */
					//fwp.write(Null_str.c_str(), record_len);
					//fwp.flush();
					if (fields[j].IsCreateIndex())									/* ����Ӧ�ֶδ�������ʱ������������ά�� */
					{
						int index_id = FindIndex(fields[j].GetFieldName());			/* �ҵ����ֶζ�Ӧ���������index_id */
						if (index_id == -1)
						{
							std::cout << "����ƥ��ʧ�ܣ�" << endl;
							continue;
						}
						indexs[index_id]->DeleteNode(records__data[j]);				/* ɾ����Ӧ��� */
					}
				}
			
				records_num--;										/* ����ɹ������м�¼������һ */
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

			/* ---------------------------------------------ƥ���ֶ���ֵ----------------------------------------------------- */
			USER_INT Record_id;									/* Record_id��¼��ǰ������¼���� */
			USER_INT offset;
			//frp.sync();
			for (USER_INT i = 0; i < select_id.size(); i++)
			{
				Record_id = select_id[i];
				offset = 0;
				std::vector<string> records__data1;				/* records__data1��¼ԭ�м�¼��Ϣ */
				std::vector<string> records__data2;				/* records__data2��¼��ǰ��Ҫ���ĵļ�¼��Ϣ */

				for (int j = 0; j < fields.size(); j++)							/* ����Ҫ���ĵļ�¼��Ϣ */
				{
					std::string r1, r2;
					frp.seekg(Record_id*record_leng + offset, ios::beg);/* ��λ���ļ�ָ�� */
					if (fields.at(j).GetFieldType() == kIntegerType)
					{
						frp.read(record__data, sizeof(char)*int_len);
						offset += true_int;
					}
					else {
						frp.read(record__data, sizeof(char)*record_len);
						offset += true_len;
					}
					records__data1.push_back(r1);
					records__data2.push_back(r2);
					records__data1[j] = record__data;
					records__data2[j] = record__data;
				}


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
								
								if (fields[k].IsCreateIndex())	/* ά������ */
								{
									int index_id = FindIndex(fields[j].GetFieldName());
									if (index_id != -1)
									{
										indexs[index_id]->UpdateNode(records__data2[index_id], records__data1[index_id]);
									}
								}
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
				offset = 0;
				for (int j = 0; j < fields.size(); j++)
				{
					fwp.seekp(Record_id*record_leng + offset, ios::beg); /* ָ�붨λ������д��λ�� */
					if (fields.at(j).GetFieldType() == kIntegerType)
					{
						fwp.write(records__data2[j].c_str(), int_len);	/* ���ֶ�˳��д���ļ� */
						offset += true_int;
					}
					else {
						fwp.write(records__data2[j].c_str(), record_len);/* ���ֶ�˳��д���ļ� */
						offset += true_len;
					}
					fwp.flush();
				}
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
	USER_INT offset = 0;
	for (auto iter = fields.begin(); iter != fields.end(); iter++)
	{
		if (iter->GetFieldName() == si.GetField())
		{
			iter->SetIsCreateIndex(true);
			type = iter->GetFieldType();
			flag = true;
			break;
		}

		if (fields.at(i).GetFieldType() == kIntegerType)
		{
			offset += true_int;
		}
		else {
			offset += true_len;
		}
		i++;
	}

	if (flag == false)
	{
		std::cout << "�ֶ�" << si.GetField() << "������." << std::endl;
		return false;
	}

	string index_path = path + "\\" + table_name + "\\index";
	if (_access(index_path.c_str(), 0))
	{
		string cmd = "md " + index_path;
		system(cmd.c_str());
	}
	index_path = index_path + "\\" + si.GetField();
	Index * temp = new Index(si.GetIndex(), si.GetField(), type, index_path);

	ofstream fip;
	std::string table_name_fields = path + "\\" + table_name + "\\" + table_name + "_fields";/* ������ͷ�ļ���table_name_fields */
	std::string is_index = "1\0";							/* is_index����Ƿ�������� */
	fip.open(table_name_fields,ios::in|ios::binary);
	fip.seekp((32 + 24 * i)*sizeof(char), ios::beg);
	fip.write(is_index.c_str(), sizeof(char)* 2);
	fip.flush();
	fip.close();

	int k = 0;
	char record_field_data[record_len];
	if (records_num != 0)
	{
		while (k < records_num)
		{
			frp.seekg((k*record_leng + offset)*sizeof(char), ios::beg);
			frp >> record_field_data;
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
	char record__data[record_len];
	std::vector<string> record__datas;
	if (Table::UseTable())
	{
		USER_INT k = 0, offset = 0, i = 0;
		//frp.sync();
		while(k < records_num)
		{
			record__datas.clear();
			bool key = false;
			offset = 0;
			for (int j = 0; j < fields.size(); j++) 
			{
				record__data[0] = '\0';
				frp.seekg(sizeof(char)*(i*record_leng+offset), ios::beg);
				if (fields.at(j).GetFieldType() == kIntegerType)
				{
					frp.read(record__data, sizeof(char)*int_len);
					offset += true_int;
				}
				else {
					frp.read(record__data, sizeof(char)*record_len);
					offset += true_len;
				}

				if (record__data[0] != '\a')
				{
					key = true;
				}
				else {
					record__data[0] = '\0';
				}
				record__datas.push_back(record__data);
			}

			if (key == true)
			{
				std::cout << "------ No." << k+1 << " ------" << endl;
				for (int j = 0; j < fields.size(); j++)
				{
					std::cout << fields.at(j).GetFieldName() << ":" << record__datas.at(j) << "  " << endl;
				}
				k++;
			}	
			i++;
		}
		std::cout << endl;
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
bool Table::Display(USER_INT id,USER_INT iter)
{
	char record__data[record_len];
	if (UseTable())
	{
		//frp.sync();
		std::cout << "------ No." << iter+1 << " ------" << endl;
		USER_INT offset = 0;
		for (int j = 0; j < fields.size(); j++)
		{
			frp.seekg(sizeof(char)*(id*record_leng + offset), ios::beg);
			if (fields.at(j).GetFieldType() == kIntegerType)
			{
				frp.read(record__data, sizeof(char)*int_len);
				offset += true_int;
			}
			else {
				frp.read(record__data, sizeof(char)*record_len);
				offset += true_len;
			}

			std::cout << fields.at(j).GetFieldName() << ":" << record__data << "  " << endl;
		}
		return true;
	}
	else {
		std::cout << "����ʧ�ܣ�" << endl;
		return false;
	}
}

/**
*  \brief ˳�����
*/
bool Table::OrderSelect(string select_field, Value select_value, OperatorType select_op)
{
	real_id.clear();
	indexs.at(0)->ShowAllId(real_id);

	std::string field = select_field;
	Value value = select_value;
	OperatorType op = select_op;
	char record__data[record_len];
	int field_id = 0;
	USER_INT count = 0, k = 0, i = 0, offset = 0,length;

	while (fields.at(field_id).GetFieldName() != field || fields.at(field_id).GetFieldType() != value.GetValueType())
	{
		if (fields.at(field_id).GetFieldType() == kIntegerType)
		{
			offset += true_int;
		}
		else {
			offset += true_len;
		}
		field_id++;
		if (field_id >= fields.size()) break;
	}

	if (field_id == fields.size())
	{
		std::cout << "�ֶ��������������޷�ƥ�䣡" << endl;
		return false;
	}
	else
	{
		if (fields.at(field_id).GetFieldType() == kIntegerType)
		{
			length = true_int;
		}
		else {
			length = true_len;
		}

		while (k < records_num)
		{
			i = real_id.at(k++);
			frp.seekg(sizeof(char)*(i*record_leng + offset), ios::beg);
			frp.read(record__data, length*sizeof(char));
			if (record__data[0] == '\0')
			{
				continue;
			}
			switch (op)
			{
			case kOpEqual:
				if (record__data == value.GetValueData())
				{
					select_id.push_back(i);
					count++;
				}
				break;
			case kOpGreater:
				if (value.GetValueType()==kStringType)
				{
					if (record__data > value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int,value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int > value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			case kOpLess:
				if (value.GetValueType() == kStringType)
				{
					if (record__data < value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int, value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int < value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			case kOpNotEqual:
				if (record__data != value.GetValueData())
				{
					select_id.push_back(i);
					count++;
				}
				break;
			case kOpGreterOrEqual:
				if (value.GetValueType() == kStringType)
				{
					if (record__data >= value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int, value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int >= value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			case kOpLessOrEqual:
				if (value.GetValueType() == kStringType)
				{
					if (record__data <= value.GetValueData())
					{
						select_id.push_back(i);
						count++;
					}
				}
				else
				{
					int record_data_int, value_data_int;
					record_data_int = atoi(record__data);
					value_data_int = atoi(value.GetValueData().c_str());
					if (record_data_int <= value_data_int)
					{
						select_id.push_back(i);
						count++;
					}
				}
				break;
			default:
				std::cout << "������ƥ��ʧ�ܣ�" << endl;
				return false;
			}
		}

		if (!count)
		{
			std::cout << "�����ڷ������������ݣ�" << endl;
			return false;
		}

		return true;
	}
}