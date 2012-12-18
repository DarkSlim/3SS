//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _TAML_XML_FILE_VISITOR_H_
#define _TAML_XML_FILE_VISITOR_H_

#ifndef _SIMBASE_H_
#include "sim/simBase.h"
#endif

#ifndef _TAML_XMLPARSER_H_
#include "persistence//taml/tamlXmlParser.h"
#endif

//-----------------------------------------------------------------------------

class TamlXmlFileVisitor : public SimObject, public TamlXmlVisitor
{
private:
    typedef SimObject Parent;

protected:
	StringTableEntry mElementName;
	StringTableEntry mAttribName;
	StringTableEntry mAttribValue;
	bool mFound;

protected:
    virtual bool visit( TiXmlElement* pXmlElement, TamlXmlParser& xmlParser )
    {
		//// Notify Script.
		//if( isMethod("onVisitElement") )
		//	Con::executef(this, 1, "onVisitElement");

		StringTableEntry pName = StringTable->insert( StringTable->EmptyString );
		// Iterate attributes.
		for ( TiXmlAttribute* pAttribute = pXmlElement->FirstAttribute(); pAttribute; pAttribute = pAttribute->Next() )
		{
			if ( dStrcmp(pAttribute->Name(), "Name") || dStrcmp(pAttribute->Name(), "name") )
				pName = StringTable->insert(pAttribute->Value());
			// Insert attribute name.
			StringTableEntry attributeName = StringTable->insert( pAttribute->Name() );
			StringTableEntry attributeValue = StringTable->insert( pAttribute->Value() );
			if ( attributeName == mAttribName )
			{
				if ( attributeValue == mAttribValue )
				{
					mElementName = pName;
					mFound = true;
					break;
				}
			}
		}
		return true;
    }

    virtual bool visit( TiXmlAttribute* pAttribute, TamlXmlParser& xmlParser )
    {
        // Sanity!
        AssertFatal( pAttribute != NULL, "Cannot search NULL attributes." );

		//// Notify Script.
		//if( isMethod("onVisitAttribute") )
		//	Con::executef(this, 1, "onVisitAttribute");

		StringTableEntry attributeName = StringTable->insert( pAttribute->Name() );
		StringTableEntry attributeValue = StringTable->insert( pAttribute->Value() );
		if ( attributeName == mAttribName && attributeValue == mAttribValue )
			mFound = true;

		return true;
    }

public:
	TamlXmlFileVisitor() : mFound(false), mElementName( StringTable->EmptyString ),
			mAttribName( StringTable->EmptyString ), mAttribValue( StringTable->EmptyString ) {}
    virtual ~TamlXmlFileVisitor() {}

    bool parse( const char* pFilename )
    {
        TamlXmlParser parser;
        return parser.parse( pFilename, *this, false );
    }

	bool findElementByAttribValue( const char*, const char*, const char* );
	void clearFound(void);

    virtual bool onAdd() { if ( !Parent::onAdd() ) return false; return true; }
    virtual void onRemove() { Parent::onRemove(); }
    static void initPersistFields();

	/// Declare Console Object.
    DECLARE_CONOBJECT( TamlXmlFileVisitor );
};

#endif // _TAML_XML_FILE_VISITOR_H_
