class Landmark
{
public:
	~Landmark()
	{
		cout << "Destroying the " << type() << " " << name() << "." << endl;
	}

	string color() const
	{
		return _color;
	}

	string name() const
	{
		return _name;
	}

	string icon() const
	{
		return _icon;
	}

	string type() const
	{
		return _type;
	}

	void setIcon(string icon)
	{
		_icon = icon;
	}

	void setColor(string color)
	{
		_color = color;
	}

	void setType(string type)
	{
		_type = type;
	}
protected:
	Landmark(string name)
	{
		_name = name;
	}
private:
	string _color;
	string _icon;
	string _name;
	string _type;
};

class Hotel : public Landmark
{
public:
	Hotel(string name)
		:Landmark(name)
	{
		setType("hotel");
		setIcon("bed");
		setColor("yellow");
	}
};

class Restaurant : public Landmark
{
public:
	Restaurant(string name, int size)
		:Landmark(name)
	{
		setIcon("small knife/fork");
		setColor("yellow");
		if (size >= 40)
		{
			setIcon("large knife/fork");
		}
		setType("restaurant");
	}
};

class Hospital : public Landmark
{
public:
	Hospital(string name)
		:Landmark(name)
	{
		setType("hospital");
		setIcon("H");
		setColor("blue");
	}
};
