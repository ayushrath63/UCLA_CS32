void listAll(const Class* c, string path)  // two-parameter overload
{
	if ((c->subclasses()).size() == 0)
	{
		cout << path + c->name() << endl;
		return;
	}
	path += c->name();
	cout << path << endl;
	for (vector<Class*>::const_iterator it = (c->subclasses()).begin(); it != (c->subclasses()).end(); ++it)
	{
		listAll(*it, path + "=>");
	}
}
