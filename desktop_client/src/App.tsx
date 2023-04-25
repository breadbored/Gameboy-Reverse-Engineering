import { useState } from 'react'
import './App.css'
import Sidebar from './components/Sidebar'
import Content from './components/Content'

function App() {
  const [page, setPage] = useState(0)
  const setPageNum = (pageNum: number) => setPage(pageNum)

  const pages = [
    {
      id: 0,
      name: "Gameboy Printer",
    },
    {
      id: 1,
      name: "Pokémon Trader",
    },
    {
      id: 2,
      name: "GB Sniffer",
    },
    {
      id: 3,
      name: "Online",
    },
  ];

  return (
    <>
      <Sidebar page={page} setPage={setPageNum} pages={pages} />
      <Content page={page} pages={pages} />
    </>
  )
}

export default App
