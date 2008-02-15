/***************************************************************************
 *   Copyright (C) 2007 by Kai Winter   *
 *   kaiwinter@gmx.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "linesandpoints.h"
/*!
 * \example linesandpoints.cpp
 * This application demonstrates how to use Geometry objects and how to add them to a layer.
 * 
 * Here are used three different point types:
 *  - One which displays a image
 *  - One which draws a plain circle
 *  - One which uses a QPen to draw a circle
 *  - One which has no markers
 * Then these Points were added to a LineString
 * 
 * Also there is a keylistener.
 * 
 * You can find this example here: MapAPI/Samples/LinesAndPoints
 * \image html sample_linesandpoints.png "screenshot"
 */
LinesAndPoints::LinesAndPoints(QWidget *parent)
	: QWidget(parent)
{
	// the size which the QMapControl should fill
	QSize size = QSize(270,250);
	
	mc = new MapControl(size);
	
	MapControl* mc2 = new MapControl(size);
	MapControl* mc3 = new MapControl(size);
	MapControl* mc4 = new MapControl(size);
	
	// create layout
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(mc);
	layout->addWidget(mc2);
	layout->addWidget(mc3);
	layout->addWidget(mc4);
	setLayout(layout);
	
	// create layer
	TileMapAdapter* mapadapter = new TileMapAdapter("tile.openstreetmap.org", "/%1/%2/%3.png", 256,0,17);
	TileMapAdapter* mapadapter2 = new TileMapAdapter("tile.openstreetmap.org", "/%1/%2/%3.png", 256,0,17);
	TileMapAdapter* mapadapter3 = new TileMapAdapter("tile.openstreetmap.org", "/%1/%2/%3.png", 256,0,17);
	TileMapAdapter* mapadapter4 = new TileMapAdapter("tile.openstreetmap.org", "/%1/%2/%3.png", 256,0,17);
	Layer* l = new Layer("Custom Layer", mapadapter, Layer::MapLayer);
	Layer* l2 = new Layer("Custom Layer", mapadapter2, Layer::MapLayer);
	Layer* l3 = new Layer("Custom Layer", mapadapter3, Layer::MapLayer);
	Layer* l4 = new Layer("Custom Layer", mapadapter4, Layer::MapLayer);
	mc->addLayer(l);
	mc2->addLayer(l2);
	mc3->addLayer(l3);
	mc4->addLayer(l4);
// 	Layer* l = new Layer("Custom Layer", mapadapter, Layer::MapLayer);
// 	mc->addLayer(l);
	
	// create a LineString
	QList<Point*> points;
	// Points with image
	points.append(new ImagePoint(8.259959, 50.001781, "images/bus_stop.png", "Mainz, Hauptbahnhof", Point::BottomLeft));
	points.append(new ImagePoint(8.263758, 49.998917, "images/bus_stop.png", "Mainz, Münsterplatz", Point::BottomLeft));
	points.append(new ImagePoint(8.265812, 50.001952, "images/bus_stop.png","Mainz, Neubrunnenplatz", Point::BottomLeft));
	// Points with a circle
	points.append(new CirclePoint(8.2688, 50.004015, "Mainz, Bauhofstraße LRP", Point::Middle));
	points.append(new CirclePoint(8.272845, 50.00495, "Mainz, Landtag", Point::Middle));
	points.append(new CirclePoint(8.280349, 50.008173, "Mainz, Brückenkopf", Point::Middle));
	// A QPen can be used to customize the 
	QPen* pointpen = new QPen(QColor(0,255,0));
	pointpen->setWidth(3);
	points.append(new CirclePoint(8.273573, 50.016315, 15, "Wiesbaden-Mainz-Kastel, Eleonorenstraße", Point::Middle, pointpen));
	points.append(new CirclePoint(8.275145, 50.016992, 15, "Wiesbaden-Mainz-Kastel, Johannes-Goßner-Straße", Point::Middle, pointpen));
	points.append(new CirclePoint(8.270476, 50.021426, 15, "Wiesbaden-Mainz-Kastel, Ruthof", Point::Middle, pointpen));
	// "Blind" Points
	points.append(new Point(8.266445, 50.025913, "Wiesbaden-Mainz-Kastel, Mudra Kaserne"));
	points.append(new Point(8.260378, 50.030345, "Wiesbaden-Mainz-Amoneburg, Dyckerhoffstraße"));

	// A QPen also can use transparency
	QPen* linepen = new QPen(QColor(0, 0, 255, 100));
	linepen->setWidth(5);
	// Add the Points and the QPen to a LineString 
	LineString* ls = new LineString(points, "Busline 54", linepen);
	
	// Add the LineString to the layer
	l->addGeometry(ls);
	
	// Connect click events of the layer to this object
	connect(l, SIGNAL(geometryClickEvent(Geometry*, QPoint)),
			  this, SLOT(geometryClickEvent(Geometry*, QPoint)));
	
	// Sets the view to the interesting area
	QList<QPointF> view;
	view.append(QPointF(8.24764, 50.0319));
	view.append(QPointF(8.28412, 49.9998));
	mc->setView(view);
	
	addZoomButtons();
}

void LinesAndPoints::addZoomButtons()
{
	// create buttons as controls for zoom
	QPushButton* zoomin = new QPushButton("+");
	QPushButton* zoomout = new QPushButton("-");
	zoomin->setMaximumWidth(50);
	zoomout->setMaximumWidth(50);
	
	connect(zoomin, SIGNAL(clicked(bool)),
			  mc, SLOT(zoomIn()));
	connect(zoomout, SIGNAL(clicked(bool)),
			  mc, SLOT(zoomOut()));
	
	// add zoom buttons to the layout of the MapControl
	QVBoxLayout* innerlayout = new QVBoxLayout;
	innerlayout->addWidget(zoomin);
	innerlayout->addWidget(zoomout);
	mc->setLayout(innerlayout);
}

void LinesAndPoints::geometryClickEvent(Geometry* geom, QPoint coord_px)
{
	qDebug() << "parent: " << geom->getParentGeometry();
	qDebug() << "Element clicked: " << geom->getName();
	if (geom->hasClickedPoints())
	{
		QList<Geometry*> pp = geom->getClickedPoints();
		qDebug() << "number of child elements: " << pp.size();
		for (int i=0; i<pp.size(); i++)
		{
			QMessageBox::information(this, geom->getName(), pp.at(i)->getName());
		}
	}
	else if (geom->GeometryType == "Point")
	{
		QMessageBox::information(this, geom->getName(), "just a point");
	}
}

LinesAndPoints::~LinesAndPoints()
{
}

void LinesAndPoints::keyPressEvent(QKeyEvent* evnt)
{
	if (evnt->key() == 49 || evnt->key() == 17825792)  // tastatur '1'
	{
		mc->zoomIn();
	}
	else if (evnt->key() == 50)
	{
		mc->moveTo(QPointF(8.25, 60));
	}
	else if (evnt->key() == 51 || evnt->key() == 16777313)     // tastatur '3'
	{
		mc->zoomOut();
	}
	else if (evnt->key() == 54) // 6
	{
		mc->setView(QPointF(8,50));
	}
	else if (evnt->key() == 16777234) // left
	{
		mc->scrollLeft();
	}
	else if (evnt->key() == 16777236) // right
	{
		mc->scrollRight();
	}
	else if (evnt->key() == 16777235 ) // up
	{
		mc->scrollUp();
	}
	else if (evnt->key() == 16777237) // down
	{
		mc->scrollDown();
	}
	else if (evnt->key() == 48 || evnt->key() == 17825797) // 0
	{
		emit(close());
	}
	else
	{
		qDebug() << evnt->key() << endl;
	}
}