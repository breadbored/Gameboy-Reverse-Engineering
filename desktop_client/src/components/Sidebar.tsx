const Sidebar = ({
  page,
  setPage,
  pages,
}: {
  page: number;
  setPage: (pageNum: number) => void;
  pages: { name: string }[];
}) => {
  return (
    <div className="sidebar">
      <h3 className="sidebar-header">BreadCodes GB Tool</h3>
      <div className="sidebar-content">
        {pages.map((p, i) => (
          <h4
            key={`sidebar-${i}`}
            className={`list-item ${page == i ? "active" : ""}`}
            onClick={() => setPage(i)}
          >
            {p.name}
          </h4>
        ))}
      </div>
    </div>
  );
};

export default Sidebar;
