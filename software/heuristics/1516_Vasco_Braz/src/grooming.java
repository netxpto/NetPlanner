
/* ******************************************************************************
 * Copyright (c) 2013-2014 Pablo Pavon-Marino, Jose-Luis Izquierdo-Zaragoza.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser Public License v3
 * which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl.html
 *
 * Contributors:
 *     Pablo Pavon-Marino, Jose-Luis Izquierdo-Zaragoza - initial API and implementation
 ***************************************************************************** */

/**
 * @author Vasco Braz, Adolfo Oliveira
 * @version 2.0, May 2015
 */

import com.net2plan.interfaces.networkDesign.IAlgorithm;
import com.net2plan.interfaces.networkDesign.Net2PlanException;
import com.net2plan.interfaces.networkDesign.NetPlan;
import com.net2plan.interfaces.networkDesign.NetworkLayer;
import com.net2plan.interfaces.networkDesign.Demand;
import com.net2plan.interfaces.networkDesign.Route;
import com.net2plan.interfaces.networkDesign.ProtectionSegment;
import com.net2plan.interfaces.networkDesign.Node;
import com.net2plan.utils.Constants.RoutingType;
import com.net2plan.utils.Triple;
import com.net2plan.interfaces.networkDesign.Link;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import cern.colt.matrix.tdouble.DoubleFactory1D;
import cern.colt.matrix.tdouble.DoubleMatrix1D;

public class grooming implements IAlgorithm {

	private NetworkLayer lowerLayer, upperLayer;

	private int maxOpticalReach;

	private void readFile() throws IOException {

		Scanner scan;
		File file = new File("C:\Users\ejfernandes\Desktop\Tese\Algoritmos Net2Plan\src\opticalReach.txt");

		try {
			scan = new Scanner(file);
			while (scan.hasNext()) {
				maxOpticalReach = scan.nextInt();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	@Override
	public String executeAlgorithm(NetPlan netPlan, Map<String, String> algorithmParameters, Map<String, String> net2planParameters) {

		System.out.println("########");

		lowerLayer = netPlan.getNetworkLayer(0);
		upperLayer = netPlan.getNetworkLayer(1);

		/* Initialization */
		int N = netPlan.getNumberOfNodes();
		int E = netPlan.getNumberOfLinks(lowerLayer);
		int D = netPlan.getNumberOfDemands(lowerLayer);

		System.out.println("Number of nodes: " + N);
		System.out.println("Number of links: " + E);
		System.out.println("Number of demands: " + D);

		netPlan.removeAllUnicastRoutingInformation();
		netPlan.setRoutingType(RoutingType.SOURCE_ROUTING, upperLayer);
		netPlan.setRoutingType(RoutingType.SOURCE_ROUTING, lowerLayer);

		if (N == 0 || E == 0 || D == 0)
			throw new Net2PlanException("This algorithm requires a topology and a demand set");

		String shortestPathType = algorithmParameters.get("shortestPathType");
		String numberOfRoutes = algorithmParameters.get("numberofroutes");
		String strProtection = algorithmParameters.get("protection");

		if (!shortestPathType.equalsIgnoreCase("hops") && !shortestPathType.equalsIgnoreCase("km"))
			throw new Net2PlanException("'shortestPathType' must be 'hops' or 'km'");

		netPlan.removeAllRoutes(lowerLayer);
		netPlan.removeAllProtectionSegments(lowerLayer);
		netPlan.removeAllRoutes(upperLayer);
		netPlan.removeAllProtectionSegments(upperLayer);

		ArrayList<Long> linkIds = netPlan.getLinkIds(lowerLayer);
		final DoubleMatrix1D linkCostVector = shortestPathType.equalsIgnoreCase("hops") ? DoubleFactory1D.dense.make(E, 1.0) : netPlan.getVectorLinkLengthInKm();
		long wavelengthCapacity = Long.parseLong(algorithmParameters.get("wavelengthCapacity"));
		boolean protection = strProtection.equalsIgnoreCase("yes") ? true : false;

		System.out.println("PROTECTION: " + protection);

		Route save = null;
		String type = upperLayer.getName();

		if (type != "Logical Topology Opaque" && type != "Logical Topology Transparent" && type != "Logical Topology Translucent")
			throw new Net2PlanException("Logical Topology Algorithm Required");

		netPlan.addRoutesFromCandidatePathList(linkCostVector.toArray(), "K", numberOfRoutes);
		System.out.println("Number of Routes: " + netPlan.getNumberOfRoutes());

		for (long links : linkIds) {
			netPlan.getLinkFromId(links).removeAllAttributes();
			netPlan.getLinkFromId(links).setCapacity(0);
		}

		switch (type) {
		case "Logical Topology Opaque":

			for (Demand d : netPlan.getDemands(lowerLayer)) {

				boolean odd = true;
				int counter = 0;

				Set<Route> droutes = d.getRoutes();
				System.out.println("droutes: " + droutes.size());

				for (Route c : droutes) {
					counter++;
					boolean jump = false;

					if (odd) {
						c.setCarriedTraffic(d.getOfferedTraffic(), d.getOfferedTraffic());
						save = c;
						odd = false;
						System.out.println("Roots");
					} else {
						if (protection) {
							List<Link> workingpath = save.getSeqLinksRealPath();
							System.out.println("Protection");

							for (Link t : workingpath) {
								if (c.getSeqLinksRealPath().contains(t)) {
									jump = true;
									break;
								}
							}

							if (jump == false) {
								ProtectionSegment segment = netPlan.addProtectionSegment(c.getSeqLinksRealPath(), d.getOfferedTraffic(), null);
								save.addProtectionSegment(segment);
								odd = true;
								break;
							}

							if (jump == true && counter == droutes.size()) {
								ProtectionSegment segment = netPlan.addProtectionSegment(c.getSeqLinksRealPath(), d.getOfferedTraffic(), null);
								save.addProtectionSegment(segment);
								odd = true;
								throw new Net2PlanException("Number of routes is not enough");
							}
						}
					}
				}
			}

			Link p;

			for (long e : linkIds) {
				p = netPlan.getLinkFromId(e);
				double sumTraffic = p.getCarriedTrafficNotIncludingProtectionSegments() + p.getReservedCapacityForProtection();
				int nw = (int) (Math.ceil(sumTraffic / wavelengthCapacity));
				String numberWavelengths = String.valueOf(nw);
				p.setCapacity(nw * wavelengthCapacity);
				p.setAttribute("nW", numberWavelengths);
			}
			break;

		case "Logical Topology Transparent":

			for (Demand d : netPlan.getDemands(lowerLayer)) {
				boolean odd = true;
				int counter = 0;

				Set<Route> droutes = d.getRoutes();
				System.out.println(droutes.size());

				for (Route c : droutes) {
					counter++;
					boolean jump = false;
					
					if (odd) {
						c.setCarriedTraffic(d.getOfferedTraffic(), d.getOfferedTraffic());
						save = c;
						odd = false;
						System.out.println("Roots");
					} else {
						if (protection) {
							List<Link> workingpath = save.getSeqLinksRealPath();
							System.out.println("Protection-Transparent");
							for (Link t : workingpath) {
								if (c.getSeqLinksRealPath().contains(t)) {
									jump = true;
									break;
								}
							}

							if (jump == false) {
								ProtectionSegment segment = netPlan.addProtectionSegment(c.getSeqLinksRealPath(), d.getOfferedTraffic(), null);
								save.addProtectionSegment(segment);
								odd = true;
								break;
							}

							if (jump == true && counter == droutes.size()) {
								ProtectionSegment segment = netPlan.addProtectionSegment(c.getSeqLinksRealPath(), d.getOfferedTraffic(), null);
								save.addProtectionSegment(segment);
								odd = true;
								throw new Net2PlanException("Number of routes is not enough");
							}
						}
					}
				}
			}

			netPlan.removeAllRoutesUnused(1);

			ArrayList<Long> tNodeIds = netPlan.getNodeIds();
			Node in;
			Node out;
			Set<Route> groomRoute;
			Set<ProtectionSegment> protectRoutes;
			Route compare = null;
			ProtectionSegment compare1 = null;
			List<Link> path;
			int nW = 0;

			for (long tNodeId : tNodeIds) {
				in = netPlan.getNodeFromId(tNodeId);
				for (long tNodeId1 : tNodeIds) {

					if (tNodeId == tNodeId1)
						continue;

					out = netPlan.getNodeFromId(tNodeId1);
					double totaltraffic = 0;

					groomRoute = netPlan.getNodePairRoutes(in, out, false, lowerLayer);
					protectRoutes = netPlan.getNodePairProtectionSegments(in, out, false, lowerLayer);

					for (Route d : groomRoute) {
						totaltraffic = totaltraffic + d.getCarriedTraffic();
						compare = d;
					}

					path = compare.getSeqLinksRealPath();

					for (Link link : path) {
						String nw = link.getAttribute("nW");
						nW = 0;

						if (nw != null) {
							nW = Integer.parseInt(nw);
							nW = (int) (nW + Math.ceil(totaltraffic / wavelengthCapacity));
							link.setAttribute("nW", String.valueOf(nW));
						} else {
							nW = (int) Math.ceil(totaltraffic / wavelengthCapacity);
							link.setAttribute("nW", String.valueOf(nW));
						}
					}

					// Protection Segments
					totaltraffic = 0;
					System.out.println("POINT5");

					for (ProtectionSegment protect : protectRoutes) {
						totaltraffic = totaltraffic + protect.getReservedCapacityForProtection();
						compare1 = protect;
					}

					System.out.println("POINT5B");
					if (protection) {
						path = compare1.getSeqLinks();
					}

					System.out.println("POINT6");
					for (Link link : path) {
						String nw = link.getAttribute("nW");
						if (nw != null) {
							nW = Integer.parseInt(nw);
							nW = (int) (nW + Math.ceil(totaltraffic / wavelengthCapacity));
							link.setAttribute("nW", String.valueOf(nW));
						} else {
							nW = (int) Math.ceil(totaltraffic / wavelengthCapacity);
							link.setAttribute("nW", String.valueOf(nW));
						}
					}
				}
			}
			break;

		case "Logical Topology Translucent":

			try {
				readFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			int hops = 0;
			Set<Route> nRoutes = new HashSet<Route>();
			
			for (Demand d : netPlan.getDemands(lowerLayer)) {
				
				nRoutes = d.getRoutes();
				for (Route c : nRoutes) {
					hops += c.getNumberOfHops();
				}
			}
			int n = hops/netPlan.getNumberOfRoutes(lowerLayer);
			
			for (Demand d : netPlan.getDemands(lowerLayer)) {
				boolean odd = true;
				int counter = 0;
				
				Set<Route> droutes = d.getRoutes();
				System.out.println(droutes.size());
				
				for (Route c : droutes) {
					counter++;
					boolean jump = false;
				
					/*List<Link> workpath = c.getSeqLinksRealPath();
					int i = 0;
					double length = 0;
					Node origin = null;
					Node dest;
					Link xpto;
					List<Link> links = new LinkedList<Link>();
					Route r;
					List<Route> listRoutes = new LinkedList<Route>();
					Route droutes2;
					
					for(Link j : workpath){
						if(i == 0){
							origin = j.getOriginNode();
						}
						length = length + j.getLengthInKm();
						if(length > maxOpticalReach){
							dest = j.getDestinationNode();
																	 
							xpto = netPlan.addLink(origin, dest, 0, length - maxOpticalReach, 200000, null, lowerLayer);
							
							links.add(xpto);
							r = netPlan.addRoute(d, d.getOfferedTraffic(), 100, links, null);
							listRoutes.add(r);
							i = 0;
						}
						i++;
						workpath = links;
					}*/				
					
					if (odd) {
						if (c.getNumberOfHops() < (n-1) && c.getLengthInKm() <= maxOpticalReach) {
							c.setCarriedTraffic(d.getOfferedTraffic(), d.getOfferedTraffic());
							save = c;
							System.out.println("Roots");
						}
						else if (c.getNumberOfHops() > (n-1) && c.getLengthInKm() <= maxOpticalReach) {
							save = c;
							System.out.println("Roots");
						}
						odd = false;
					} else {
						if (protection) {
							List<Link> workingpath = save.getSeqLinksRealPath();
							
							System.out.println("Protection-Translucent");
							for (Link t : workingpath) {
								if (c.getSeqLinksRealPath().contains(t)) {
									jump = true;
									break;
								}
							}

							if (jump == false) {
								if (c.getNumberOfHops() <= (n-1) && c.getLengthInKm() <= maxOpticalReach) {
									ProtectionSegment segment = netPlan.addProtectionSegment(c.getSeqLinksRealPath(), d.getOfferedTraffic(), null);
									save.addProtectionSegment(segment);
								}
								odd = true;
								break;
							}

							if (jump == true && counter == droutes.size()) {
								ProtectionSegment segment = netPlan.addProtectionSegment(c.getSeqLinksRealPath(), d.getOfferedTraffic(), null);
								save.addProtectionSegment(segment);
								odd = true;
								throw new Net2PlanException("Number of routes is not enough");
							}
						}
					}
				}
			}

			netPlan.removeAllRoutesUnused(1);

			ArrayList<Long> tNodeIdsTransl = netPlan.getNodeIds();
			Node inTransl;
			Node outTransl;
			Set<Route> groomRouteTransl;
			Set<ProtectionSegment> protectRoutesTransl;
			Route compareTransl = null;
			Route compareTransl2 = null;
			ProtectionSegment compare1Transl = null;
			List<Link> pathTransl;
			List<Link> pathTransl2;
			int nWTransl = 0;

			for (long tNodeId : tNodeIdsTransl) {
				inTransl = netPlan.getNodeFromId(tNodeId);
				for (long tNodeId1 : tNodeIdsTransl) {

					if (tNodeId == tNodeId1)
						continue;

					outTransl = netPlan.getNodeFromId(tNodeId1);
					double totaltraffic = 0;

					groomRouteTransl = netPlan.getNodePairRoutes(inTransl, outTransl, false, lowerLayer);
					protectRoutesTransl = netPlan.getNodePairProtectionSegments(inTransl, outTransl, false, lowerLayer);

					for (Route c : groomRouteTransl) {
						if(c.getCarriedTraffic() <= c.getOccupiedCapacity()){
							compareTransl = c;
						}
					}
					
					pathTransl = compareTransl.getSeqLinksRealPath();
					
					for(Route c : groomRouteTransl){
						for (Link link : pathTransl){
							if(link.getUtilizationNotIncludingProtectionSegments() < 1){
								totaltraffic = totaltraffic + c.getCarriedTraffic();
								compareTransl2 = c;
							}
							else
								compareTransl = c;
						}
					}
					
					pathTransl2 = compareTransl2.getSeqLinksRealPath();
							
					for (Link link : pathTransl2) {
						String nw = link.getAttribute("nW");
						nWTransl = 0;

						if (nw != null) {
							nWTransl = Integer.parseInt(nw);
							nWTransl = (int) (nWTransl + Math.ceil(totaltraffic / wavelengthCapacity));
							link.setAttribute("nW", String.valueOf(nWTransl));
						} else {
							nWTransl = (int) Math.ceil(totaltraffic / wavelengthCapacity);
							link.setAttribute("nW", String.valueOf(nWTransl));
						}
					}

					// Protection Segments
					totaltraffic = 0;
					//System.out.println("POINT5");

					for (ProtectionSegment protect : protectRoutesTransl) {
						if(protect.getCarriedTraffic() <= protect.getCapacity()){
							totaltraffic = totaltraffic + protect.getReservedCapacityForProtection();
							compare1Transl = protect;
						}
					}

					//System.out.println("POINT5B");
					if (protection) {
						pathTransl = compare1Transl.getSeqLinks();
					}

					//System.out.println("POINT6");
					for (Link link : pathTransl) {
						String nw = link.getAttribute("nW");
						if (nw != null) {
							nWTransl = Integer.parseInt(nw);
							nWTransl = (int) (nWTransl + Math.ceil(totaltraffic / wavelengthCapacity));
							link.setAttribute("nW", String.valueOf(nWTransl));
						} else {
							nWTransl = (int) Math.ceil(totaltraffic / wavelengthCapacity);
							link.setAttribute("nW", String.valueOf(nWTransl));
						}
					}
				}
			}

			for (long e : linkIds) {
				p = netPlan.getLinkFromId(e);
				String n_w = p.getAttribute("nW");
				if (n_w == null)
					p.setCapacity(0);
				else {
					int numberWavelengths = Integer.parseInt(n_w);
					p.setCapacity(numberWavelengths * wavelengthCapacity);
				}
			}
			
		}

		return "Ok!";
	}

	public List<Triple<String, String, String>> getParameters() {
		List<Triple<String, String, String>> parameters = new ArrayList<Triple<String, String, String>>();
		parameters.add(Triple.of("shortestPathType", "hops", "Each demand is routed according to the shortest path type. Can be 'km' or 'hops'."));
		parameters.add(Triple.of("wavelengthCapacity", "100", "Capacity per wavelength."));
		parameters.add(Triple.of("numberofroutes", "10", "Total number of routes per demand."));
		parameters.add(Triple.of("protection", "no", "1+1 protection (yes/no)."));
		return parameters;
	}

	@Override
	public String getDescription() {
		return "Algorithm that creates routes and protection paths based on the shortestPath (hops or km) making sure they are bidirectional and according to the logical topology. Link capacity is based on the number of wavelengths necessary with a user defined wavelength traffic capacity.";
	}

}