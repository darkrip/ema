#ifndef __FILE_ATTR_HPP__
#define __FILE_ATTR_HPP__


/*
_Out_opt_ LPFILETIME lpCreationTime,
_Out_opt_ LPFILETIME lpLastAccessTime,
_Out_opt_ LPFILETIME lpLastWriteTime


_P ;  n - им€ файла
_P ;  z - размер не сжатого файла
_P ;  c - описание файла
_P ;  p - размер сжатого файла
_P ;  d - день
_P ;  t - мес€ц
_P ;  TTT - трехбуквенное им€ мес€ца (Jan, Feb, Mar, ...)
_P ;  y - год
_P ;  h - часы
_P ;  H - буква - модификатор часов (a - врем€ до полудн€ (a.m. time),
_P                                   p - врем€ после полудн€ (p.m. time))
_P ;  m - минуты
_P ;  s - секунды
_P ;  a - атрибуты
_P ;  e - расширение файла
_P ;  * - пропускать до первого пробела или конца строки

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