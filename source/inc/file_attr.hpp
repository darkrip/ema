#ifndef __FILE_ATTR_HPP__
#define __FILE_ATTR_HPP__


/*
_Out_opt_ LPFILETIME lpCreationTime,
_Out_opt_ LPFILETIME lpLastAccessTime,
_Out_opt_ LPFILETIME lpLastWriteTime


_P ;  n - ��� �����
_P ;  z - ������ �� ������� �����
_P ;  c - �������� �����
_P ;  p - ������ ������� �����
_P ;  d - ����
_P ;  t - �����
_P ;  TTT - ������������� ��� ������ (Jan, Feb, Mar, ...)
_P ;  y - ���
_P ;  h - ����
_P ;  H - ����� - ����������� ����� (a - ����� �� ������� (a.m. time),
_P                                   p - ����� ����� ������� (p.m. time))
_P ;  m - ������
_P ;  s - �������
_P ;  a - ��������
_P ;  e - ���������� �����
_P ;  * - ���������� �� ������� ������� ��� ����� ������

*/





class FileAttr
{
	enum PreferedAttrs
	{
		paIsDirectory,
		paIsReadOnly,
		paIsArhived,
		paIsHidden,
		paIsSystem,
		paSize,
		paCommpressedSize,
		paDescription,
		paCreationTime,
		paLastModificationTime,
		paLastOpenTime,
	};
	
};

























#endif//__FILE_ATTR_HPP__