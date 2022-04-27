#ifndef _LAYERLIST_H_
#define _LAYERLIST_H_

#include "RenderGroup.h"
#include <QList>


class LayerList : public RenderGroup
{
public:
	LayerList();
	virtual ~LayerList();

	void addLayer(Layer* layer);

	void removeLayer(Layer* layer);

	Layer* getLayer(int index);
	/*!
	*\fn		LayerList* asLayerList()
	*\brief		ÊÇ·ñÊÇÍ¼²ã×é
	*\return	Layer*
	*/
	virtual LayerList* asLayerList() { return this; }

	int getNumLayer();
protected:
	QList<Layer*> m_layerList;

};



#endif